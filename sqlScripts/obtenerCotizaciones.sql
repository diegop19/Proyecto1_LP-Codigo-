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