DELIMITER $$

CREATE PROCEDURE eliminarFacturaSimple(IN p_idFactura INT)
BEGIN
    DELETE FROM cliente_factura WHERE idFactura = p_idFactura;
    DELETE FROM FACTURA WHERE identificadorFactura = p_idFactura;
END$$

DELIMITER ;