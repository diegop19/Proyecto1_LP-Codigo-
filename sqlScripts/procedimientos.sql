DELIMITER $$
CREATE PROCEDURE actualizarCotizacionProducto(
    IN p_idCotizacion INT,
    IN p_idProducto VARCHAR(100),
    IN p_cantidad INT
)
BEGIN
    DECLARE existe INT;
    SELECT COUNT(*) INTO existe 
    FROM COTIZACION_PRODUCTO 
    WHERE idCotizacion = p_idCotizacion 
    AND identificadorProducto = p_idProducto;
    
    IF existe > 0 THEN
        UPDATE COTIZACION_PRODUCTO
        SET cantidad = p_cantidad
        WHERE idCotizacion = p_idCotizacion
        AND identificadorProducto = p_idProducto;
    ELSE
        INSERT INTO COTIZACION_PRODUCTO (idCotizacion, identificadorProducto, cantidad)
        VALUES (p_idCotizacion, p_idProducto, p_cantidad);
    END IF;
END$$
DELIMITER ;


DELIMITER $$
CREATE PROCEDURE actualizarCantidadProducto(
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidadAumento INT
)
BEGIN
    DECLARE producto_existe INT;
    DECLARE cantidad_actual INT;
    DECLARE nueva_cantidad INT;

    SELECT COUNT(*) INTO producto_existe
    FROM PRODUCTO
    WHERE identificadorProducto = p_identificadorProducto;
    
    IF producto_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'El producto no existe';
    ELSE
        SELECT cantidadDisponible INTO cantidad_actual
        FROM PRODUCTO
        WHERE identificadorProducto = p_identificadorProducto;

        SET nueva_cantidad = cantidad_actual + p_cantidadAumento;
        
        IF nueva_cantidad < 0 THEN
			UPDATE PRODUCTO
            SET cantidadDisponible = 0
            WHERE identificadorProducto = p_identificadorProducto;
			SIGNAL SQLSTATE '01000'  -- Código para warning (no error)
            SET MESSAGE_TEXT = 'Stock insuficiente. Se ajustó a 0.';

       ELSE
			UPDATE PRODUCTO
			SET cantidadDisponible = nueva_cantidad
			WHERE identificadorProducto = p_identificadorProducto;
		END IF;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE desplegarProductosFactura(IN factura_id INT)
BEGIN
    SELECT 
        f.identificadorFactura,
        f.fechaFacturado,
        f.subtotal,
        f.total
    FROM 
        FACTURA f
    WHERE 
        f.identificadorFactura = factura_id;
    SELECT 
        fp.identificadorProducto,
        p.nombreProducto,
        fp.cantidad,
        fp.precioUnitario
    FROM 
        FACTURA_PRODUCTO fp
    JOIN 
        PRODUCTO p ON fp.identificadorProducto = p.identificadorProducto
    WHERE 
        fp.idFactura = factura_id;
        
END $$

DELIMITER ;

DELIMITER $$
CREATE PROCEDURE eliminarProductoCotizacion(
    IN p_idCotizacion INT,
    IN p_identificadorProducto VARCHAR(100)
)
BEGIN
    DELETE FROM COTIZACION_PRODUCTO
    WHERE idCotizacion = p_idCotizacion
    AND identificadorProducto = p_identificadorProducto;
END$$
DELIMITER ;


DELIMITER //

CREATE PROCEDURE sp_obtener_cotizaciones_pendientes()
BEGIN
    SELECT COUNT(*) AS total_pendientes
    FROM cotizacion 
    WHERE estadoCotizacion = 0;
END //

CREATE PROCEDURE sp_obtener_cotizaciones_facturadas()
BEGIN
    SELECT COUNT(*) AS total_facturadas
    FROM cotizacion 
    WHERE estadoCotizacion = 1;
END //

CREATE PROCEDURE sp_obtener_promedio_compras()
BEGIN
    SELECT 
        AVG(total) AS promedio,
        SUM(total) AS monto_total,
        COUNT(*) AS total_compras
    FROM factura;
END //

DELIMITER ;

DELIMITER //

CREATE PROCEDURE sp_obtener_producto_popular_familia()
BEGIN
    WITH VentasPorProducto AS (
        SELECT 
            f.id_familia,
            p.id_producto,
            SUM(dc.cantidad) AS total_vendido
        FROM tbl_detalle_cotizacion dc
        JOIN tbl_productos p ON dc.id_producto = p.id_producto
        JOIN tbl_familias f ON p.id_familia = f.id_familia
        JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
        WHERE c.estado_cotizacion = 'FACTURADA'
        GROUP BY f.id_familia, p.id_producto
    )
    SELECT 
        f.id_familia,
        f.descripcion AS familia,
        p.codigo_producto,
        p.nombre_producto,
        vpp.total_vendido
    FROM VentasPorProducto vpp
    JOIN tbl_productos p ON vpp.id_producto = p.id_producto
    JOIN tbl_familias f ON vpp.id_familia = f.id_familia
    WHERE vpp.total_vendido = (
        SELECT MAX(total_vendido)
        FROM VentasPorProducto 
        WHERE id_familia = vpp.id_familia
    );
END //

CREATE PROCEDURE sp_obtener_ventas_por_familia(
    IN p_fecha_inicio DATE,
    IN p_fecha_fin DATE
)
BEGIN
    SELECT 
        f.id_familia,
        f.descripcion AS familia,
        SUM(dc.cantidad * p.precio_venta) AS monto_total,
        SUM(dc.cantidad) AS cantidad_total
    FROM tbl_detalle_cotizacion dc
    JOIN tbl_productos p ON dc.id_producto = p.id_producto
    JOIN tbl_familias f ON p.id_familia = f.id_familia
    JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
    WHERE c.estado_cotizacion = 'FACTURADA'
    AND (p_fecha_inicio IS NULL OR c.fecha >= p_fecha_inicio)
    AND (p_fecha_fin IS NULL OR c.fecha <= p_fecha_fin)
    GROUP BY f.id_familia
    ORDER BY monto_total DESC;
END //

DELIMITER ;

DELIMITER //

CREATE PROCEDURE sp_obtener_top_productos(IN p_limit INT)
BEGIN
    SELECT 
        p.codigo_producto,
        p.nombre_producto,
        SUM(dc.cantidad) AS total_vendido,
        p.precio_venta,
        SUM(dc.cantidad * p.precio_venta) AS monto_total
    FROM tbl_detalle_cotizacion dc
    JOIN tbl_productos p ON dc.id_producto = p.id_producto
    JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
    WHERE c.estado_cotizacion = 'FACTURADA'
    GROUP BY p.id_producto
    ORDER BY total_vendido DESC
    LIMIT p_limit;
END //

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE obtenerEstadoCotizacion(IN numCotizacion INT)
BEGIN
    SELECT estadoCotizacion
    FROM COTIZACION
    WHERE numeroCotizacion = numCotizacion;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE facturarProductos(IN numCotizacion INT)
BEGIN
    DECLARE cantidadDisponible INT;
    DECLARE precioUnitario DOUBLE;
    DECLARE cantidadCotizada INT;
    DECLARE identificadorProducto VARCHAR(100);
    DECLARE subtotal DOUBLE DEFAULT 0;
    DECLARE total DOUBLE;
    DECLARE done INT DEFAULT 0;
    DECLARE idFactura INT;
    DECLARE cur CURSOR FOR
        SELECT p.identificadorProducto, cp.cantidad
        FROM COTIZACION_PRODUCTO cp
        JOIN PRODUCTO p ON cp.identificadorProducto = p.identificadorProducto
        WHERE cp.idCotizacion = numCotizacion;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;
    INSERT INTO FACTURA(fechaFacturado, subtotal, total)
    VALUES (NOW(), 0, 0); 
    SET idFactura = LAST_INSERT_ID();
    OPEN cur;
    read_loop: LOOP
        FETCH cur INTO identificadorProducto, cantidadCotizada;
        IF done THEN
            LEAVE read_loop;
        END IF;
        SELECT cantidadDisponible, precio INTO cantidadDisponible, precioUnitario
        FROM PRODUCTO
        WHERE identificadorProducto = identificadorProducto;
        IF cantidadCotizada <= cantidadDisponible THEN
            SET subtotal = subtotal + (precioUnitario * cantidadCotizada);
            INSERT INTO FACTURA_PRODUCTO(idFactura, identificadorProducto, cantidad, precioUnitario)
            VALUES (idFactura, identificadorProducto, cantidadCotizada, precioUnitario);
            UPDATE PRODUCTO
            SET cantidadDisponible = cantidadDisponible - cantidadCotizada
            WHERE identificadorProducto = identificadorProducto;
        ELSE
            SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'No hay suficiente cantidad disponible para el producto.';
        END IF;
    END LOOP;
    SET total = subtotal * 1.13;
    UPDATE FACTURA
    SET subtotal = subtotal, total = total
    WHERE identificadorFactura = idFactura;
    CLOSE cur;

END $$

DELIMITER ;


DELIMITER $$

CREATE PROCEDURE filtrarProductos(
    IN p_identificadorFamilia VARCHAR(100))
BEGIN
    -- Verificar si la familia existe
    DECLARE familia_existe INT;
    SELECT COUNT(*) INTO familia_existe
    FROM FAMILIAPRODUCTO
    WHERE identificadorFamilia = p_identificadorFamilia;
    
    IF familia_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'La familia especificada no existe';
    ELSE
        SELECT 
            p.identificadorProducto,
            p.nombreProducto,
            p.costo,
            p.precio,
            p.cantidadDisponible,
            fp.identificadorFamilia
        FROM 
            PRODUCTO p
        JOIN 
            FAMILIAPRODUCTO fp ON p.identificadorFamilia = fp.identificadorFamilia
        WHERE 
            p.identificadorFamilia = p_identificadorFamilia
        ORDER BY 
            p.nombreProducto;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE insertarCotizacionProducto(
    IN p_idCotizacion INT,
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidad INT
)
BEGIN
    INSERT INTO COTIZACION_PRODUCTO (idCotizacion, identificadorProducto, cantidad)
    VALUES (p_idCotizacion, p_identificadorProducto, p_cantidad);
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE insertarFamilia(
	IN p_identificadorFamilia VARCHAR(100),
    IN p_descripcionFamilia VARCHAR(100)
)
BEGIN
	INSERT INTO familiaproducto(identificadorFamilia, descripcionFamilia)
    VALUES (p_identificadorFamilia, p_descripcionFamilia);
END $$

DELIMITER ;

DELIMITER $$
CREATE PROCEDURE insertar_producto(
    IN p_identificadorProducto VARCHAR(100),
    IN p_identificadorFamilia VARCHAR(100),
    IN p_nombreProducto VARCHAR(100),
    IN p_costo DOUBLE,
    IN p_precio DOUBLE,
    IN p_cantidadDisponible INT
)
BEGIN
    DECLARE familia_existe INT;
    SELECT COUNT(*) INTO familia_existe
    FROM FAMILIAPRODUCTO
    WHERE identificadorFamilia = p_identificadorFamilia;
    IF familia_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'La familia de producto no existe';
    ELSE
        INSERT INTO PRODUCTO (
            identificadorProducto,
            identificadorFamilia,
            nombreProducto,
            costo,
            precio,
            cantidadDisponible
        )
        VALUES (
            p_identificadorProducto,
            p_identificadorFamilia,
            p_nombreProducto,
            p_costo,
            p_precio,
            p_cantidadDisponible
        );
    END IF;
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE insertarCotizacion(OUT p_idCotizacion INT)
BEGIN
INSERT INTO cotizacion(estadoCotizacion) VALUES (0);
SET p_idCotizacion = LAST_INSERT_ID();
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE mostrarFacturas()
BEGIN
    SELECT * FROM FACTURA;
END $$

DELIMITER ;

DELIMITER $$
CREATE PROCEDURE obtenerCotizaciones(IN numCotizacion INT)
BEGIN
    SELECT 
        p.identificadorProducto, 
        p.nombreProducto, 
        p.precio,
        cp.cantidad
    FROM 
        COTIZACION_PRODUCTO cp
    JOIN 
        PRODUCTO p ON cp.identificadorProducto = p.identificadorProducto
    WHERE 
        cp.idCotizacion = numCotizacion;



END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE obtenerFacturas()
BEGIN
    SELECT 
        identificadorFactura,
        fechaFacturado,
        subtotal,
        total
    FROM 
        FACTURA;
END $$
DELIMITER ;


DELIMITER $$
CREATE PROCEDURE ObtenerProductoIndividual(IN idProducto VARCHAR(100))
BEGIN
    SELECT nombreProducto, precio 
    FROM PRODUCTO
    WHERE identificadorProducto = idProducto;
END $$
DELIMITER ;

DELIMITER $$

CREATE PROCEDURE obtenerProductos()
BEGIN
    SELECT * FROM PRODUCTO ORDER BY nombreProducto;
END $$

DELIMITER ;

DELIMITER //

CREATE PROCEDURE removerProducto(
    IN identificadorProducto VARCHAR(100)
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    IF NOT EXISTS (SELECT 1 FROM productos WHERE id_producto = identificadorProducto) THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'El producto no existe';
    END IF;
    
    DELETE FROM productos 
    WHERE id_producto = identificadorProducto;
    
    COMMIT;
END //

DELIMITER ;



