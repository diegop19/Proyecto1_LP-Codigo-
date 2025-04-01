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