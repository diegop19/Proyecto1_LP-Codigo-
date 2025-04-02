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


DELIMITER $$
CREATE PROCEDURE actualizarEstadoCotizacion(
    IN numCotizacion INT, 
    OUT resultado INT
)
BEGIN
    DECLARE estadoActual BOOLEAN;

    SELECT estadoCotizacion INTO estadoActual
    FROM COTIZACION
    WHERE numeroCotizacion = numCotizacion;
    
    IF estadoActual IS NULL THEN
        SET resultado = -1;
    ELSE
        IF estadoActual = 0 THEN
            UPDATE COTIZACION 
            SET estadoCotizacion = 1 
            WHERE numeroCotizacion = numCotizacion;
            
            SET resultado = 0;
        ELSE
            SET resultado = 1;
        END IF;
    END IF;
END $$
DELIMITER ;

DELIMITER $$

CREATE PROCEDURE reabajarStock(IN codigoProducto VARCHAR(100), IN cantidadRebajar INT)
BEGIN
    UPDATE PRODUCTO
    SET cantidadDisponible = cantidadDisponible - cantidadRebajar
    WHERE identificadorProducto = codigoProducto;
    IF ROW_COUNT() = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Producto no encontrado';
    ELSE
        SELECT 'Stock actualizado exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE actualizarFactura(
    IN p_identificadorFactura INT,
    IN p_subtotal DOUBLE,
    IN p_total DOUBLE
)
BEGIN
    UPDATE FACTURA
    SET subtotal = p_subtotal, total = p_total
    WHERE identificadorFactura = p_identificadorFactura;

    IF ROW_COUNT() = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Factura no encontrada';
    ELSE
        SELECT 'Factura actualizada exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE agregarCliente(
    IN p_numCedula INT,
    IN p_nombre VARCHAR(100),
    IN p_apellido VARCHAR(100),
    OUT p_resultado INT,
    OUT p_mensaje VARCHAR(255))
BEGIN
    DECLARE persona_existe INT;
    DECLARE cliente_existe INT;
    
    SET p_resultado = 0;
    SET p_mensaje = '';

    SELECT COUNT(*) INTO persona_existe FROM PERSONA WHERE numCedula = p_numCedula;

    IF persona_existe > 0 THEN
        SELECT COUNT(*) INTO cliente_existe FROM CLIENTE WHERE cedulaCliente = p_numCedula;
    ELSE
        SET cliente_existe = 0;
    END IF;

    IF cliente_existe > 0 THEN
        SET p_resultado = -1;
        SET p_mensaje = 'Error: La persona ya está registrada como cliente';
    ELSE

        IF persona_existe = 0 THEN
            INSERT INTO PERSONA(numCedula, nombre, apellido)
            VALUES(p_numCedula, p_nombre, p_apellido);
            
            SET p_mensaje = CONCAT(p_mensaje, 'Persona agregada. ');
        END IF;

        INSERT INTO CLIENTE(cedulaCliente)
        VALUES(p_numCedula);
        
        SET p_resultado = 1;
        SET p_mensaje = CONCAT(p_mensaje, 'Cliente registrado exitosamente');
    END IF;
END$$

DELIMITER ;

DROP PROCEDURE IF EXISTS agregarFacturaCliente;
DELIMITER $$

CREATE PROCEDURE agregarFacturaCliente(
    IN p_idCliente INT,
    IN p_idFactura INT,
    OUT p_resultado INT,
    OUT p_mensaje VARCHAR(255))
BEGIN
    DECLARE cliente_existe INT;
    DECLARE factura_existe INT;
    DECLARE relacion_existe INT;

    SET p_resultado = 0;
    SET p_mensaje = '';
    

    SELECT COUNT(*) INTO cliente_existe FROM CLIENTE WHERE cedulaCliente = p_idCliente;
    
    SELECT COUNT(*) INTO factura_existe FROM FACTURA WHERE identificadorFactura = p_idFactura;
    SELECT COUNT(*) INTO relacion_existe FROM cliente_factura 
    WHERE idCliente = p_idCliente AND idFactura = p_idFactura;

    IF cliente_existe = 0 THEN
        SET p_resultado = -1;
        SET p_mensaje = 'Error: El cliente no existe';
    ELSEIF factura_existe = 0 THEN
        SET p_resultado = -2;
        SET p_mensaje = 'Error: La factura no existe';
    ELSEIF relacion_existe > 0 THEN
        SET p_resultado = -3;
        SET p_mensaje = 'Error: Esta factura ya está asociada al cliente';
    ELSE
        INSERT INTO cliente_factura(idCliente, idFactura)
        VALUES(p_idCliente, p_idFactura);
        
        SET p_resultado = 1;
        SET p_mensaje = 'Factura asociada al cliente correctamente';
    END IF;
END$$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE eliminarFacturaSimple(IN p_idFactura INT)
BEGIN
    DELETE FROM cliente_factura WHERE idFactura = p_idFactura;
    DELETE FROM FACTURA WHERE identificadorFactura = p_idFactura;
END$$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE CrearFactura()
BEGIN
    INSERT INTO FACTURA (fechaFacturado, subtotal, total)
    VALUES (CURDATE(), 0, 0);
    SELECT LAST_INSERT_ID() AS numeroFactura;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE guardarFacturaProducto(
    IN p_idFactura INT,
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidad INT,
    IN p_precioUnitario DOUBLE
)
BEGIN
    INSERT INTO FACTURA_PRODUCTO (idFactura, identificadorProducto, cantidad, precioUnitario)
    VALUES (p_idFactura, p_identificadorProducto, p_cantidad, p_precioUnitario);
    IF ROW_COUNT() = 0 THEN

        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Error al guardar el detalle de la factura-producto';
    ELSE
        SELECT 'Detalle de factura-producto guardado exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE obtenerDisponible(IN idProducto VARCHAR(100), OUT cantidadEnStock INT)
BEGIN

    SELECT cantidadDisponible
    INTO cantidadEnStock
    FROM PRODUCTO
    WHERE identificadorProducto = idProducto;

    
    IF cantidadEnStock IS NULL THEN
        SET cantidadEnStock = 0;
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE reabajarStock(IN codigoProducto VARCHAR(100), IN cantidadRebajar INT)
BEGIN
    DECLARE cantidadActual INT;
    SELECT cantidadDisponible INTO cantidadActual
    FROM PRODUCTO
    WHERE identificadorProducto = codigoProducto;
    IF cantidadActual < cantidadRebajar THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'No hay suficiente stock para rebajar la cantidad solicitada';
    ELSE
        UPDATE PRODUCTO
        SET cantidadDisponible = cantidadDisponible - cantidadRebajar
        WHERE identificadorProducto = codigoProducto;
        SELECT 'Stock actualizado exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;

DELIMITER //

CREATE PROCEDURE removerProducto(
    IN p_identificadorProducto VARCHAR(100)
)
BEGIN
    DELETE FROM producto
    WHERE identificadorProducto = p_identificadorProducto;
END //

DELIMITER ;

DELIMITER $$

CREATE PROCEDURE validarCredencialesAdmin(
    IN p_nombreUsuario VARCHAR(100),
    IN p_clave VARCHAR(50),
    OUT p_resultado INT)
BEGIN
    DECLARE v_usuario_id INT;
    DECLARE v_clave_correcta VARCHAR(50);

    SELECT idUsuario INTO v_usuario_id 
    FROM usuarioAdmin 
    WHERE nombreUsuario = p_nombreUsuario;

    IF v_usuario_id IS NOT NULL THEN
        SELECT clave INTO v_clave_correcta
        FROM contraseñaAdmin
        WHERE idUsuario = v_usuario_id;
        
        IF v_clave_correcta = p_clave THEN
            SET p_resultado = 1; 
        ELSE
            SET p_resultado = 0; 
        END IF;
    ELSE
        SET p_resultado = 0; 
    END IF;
END$$

DELIMITER ;