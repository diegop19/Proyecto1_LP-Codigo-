DELIMITER //

CREATE PROCEDURE sp_obtener_top_productos(IN p_limit INT)
BEGIN
    SELECT 
        p.codigo_producto,
        p.nombre_producto,
        SUM(dc.cantidad) AS total_vendido,
        p.precio_venta,
        SUM(dc.cantidad * p.precio_venta) AS monto_total
    FROM tbl_detalle_cotizacion dc
    JOIN tbl_productos p ON dc.id_producto = p.id_producto
    JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
    WHERE c.estado_cotizacion = 'FACTURADA'
    GROUP BY p.id_producto
    ORDER BY total_vendido DESC
    LIMIT p_limit;
END //

DELIMITER ;