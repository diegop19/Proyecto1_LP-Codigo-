
DELIMITER //

CREATE PROCEDURE sp_obtener_cotizaciones_pendientes()
BEGIN
    SELECT COUNT(*) AS total_pendientes
    FROM tbl_cotizaciones 
    WHERE estado_cotizacion = 'PENDIENTE';
END //

CREATE PROCEDURE sp_obtener_cotizaciones_facturadas()
BEGIN
    SELECT COUNT(*) AS total_facturadas
    FROM tbl_cotizaciones 
    WHERE estado_cotizacion = 'FACTURADA';
END //

CREATE PROCEDURE sp_obtener_promedio_compras()
BEGIN
    SELECT 
        AVG(total) AS promedio,
        SUM(total) AS monto_total,
        COUNT(*) AS total_compras
    FROM tbl_facturas
    WHERE estado = 'ACTIVA';
END //

DELIMITER ;