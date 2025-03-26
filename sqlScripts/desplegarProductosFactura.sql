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
