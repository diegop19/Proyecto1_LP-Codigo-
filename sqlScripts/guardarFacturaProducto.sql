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