DELIMITER $$
CREATE PROCEDURE actualizarCotizacionProducto(
    IN p_idCotizacion INT,
    IN p_idProducto VARCHAR(100),
    IN p_cantidad INT
)
BEGIN
    DECLARE existe INT;
    SELECT COUNT(*) INTO existe 
    FROM COTIZACION_PRODUCTO 
    WHERE idCotizacion = p_idCotizacion 
    AND identificadorProducto = p_idProducto;
    
    IF existe > 0 THEN
        UPDATE COTIZACION_PRODUCTO
        SET cantidad = p_cantidad
        WHERE idCotizacion = p_idCotizacion
        AND identificadorProducto = p_idProducto;
    ELSE
        INSERT INTO COTIZACION_PRODUCTO (idCotizacion, identificadorProducto, cantidad)
        VALUES (p_idCotizacion, p_idProducto, p_cantidad);
    END IF;
END$$
DELIMITER ;