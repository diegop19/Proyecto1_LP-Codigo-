DELIMITER //

CREATE PROCEDURE sp_obtener_producto_popular_familia()
BEGIN
    WITH VentasPorProducto AS (
        SELECT 
            f.id_familia,
            p.id_producto,
            SUM(dc.cantidad) AS total_vendido
        FROM tbl_detalle_cotizacion dc
        JOIN tbl_productos p ON dc.id_producto = p.id_producto
        JOIN tbl_familias f ON p.id_familia = f.id_familia
        JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
        WHERE c.estado_cotizacion = 'FACTURADA'
        GROUP BY f.id_familia, p.id_producto
    )
    SELECT 
        f.id_familia,
        f.descripcion AS familia,
        p.codigo_producto,
        p.nombre_producto,
        vpp.total_vendido
    FROM VentasPorProducto vpp
    JOIN tbl_productos p ON vpp.id_producto = p.id_producto
    JOIN tbl_familias f ON vpp.id_familia = f.id_familia
    WHERE vpp.total_vendido = (
        SELECT MAX(total_vendido)
        FROM VentasPorProducto 
        WHERE id_familia = vpp.id_familia
    );
END //

CREATE PROCEDURE sp_obtener_ventas_por_familia(
    IN p_fecha_inicio DATE,
    IN p_fecha_fin DATE
)
BEGIN
    SELECT 
        f.id_familia,
        f.descripcion AS familia,
        SUM(dc.cantidad * p.precio_venta) AS monto_total,
        SUM(dc.cantidad) AS cantidad_total
    FROM tbl_detalle_cotizacion dc
    JOIN tbl_productos p ON dc.id_producto = p.id_producto
    JOIN tbl_familias f ON p.id_familia = f.id_familia
    JOIN tbl_cotizaciones c ON dc.id_cotizacion = c.id_cotizacion
    WHERE c.estado_cotizacion = 'FACTURADA'
    AND (p_fecha_inicio IS NULL OR c.fecha >= p_fecha_inicio)
    AND (p_fecha_fin IS NULL OR c.fecha <= p_fecha_fin)
    GROUP BY f.id_familia
    ORDER BY monto_total DESC;
END //

DELIMITER ;