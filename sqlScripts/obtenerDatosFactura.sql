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
