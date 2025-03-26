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


