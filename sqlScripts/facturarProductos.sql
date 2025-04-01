DELIMITER $$

CREATE PROCEDURE CrearFactura()
BEGIN
    INSERT INTO FACTURA (fechaFacturado, subtotal, total)
    VALUES (CURDATE(), 0, 0);
    SELECT LAST_INSERT_ID() AS numeroFactura;
END $$

DELIMITER ;