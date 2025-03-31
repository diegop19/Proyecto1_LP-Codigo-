
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