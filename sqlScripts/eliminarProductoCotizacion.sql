DELIMITER $$
CREATE PROCEDURE eliminarProductoCotizacion(
    IN p_idCotizacion INT,
    IN p_identificadorProducto VARCHAR(100)
)
BEGIN
    DELETE FROM COTIZACION_PRODUCTO
    WHERE idCotizacion = p_idCotizacion
    AND identificadorProducto = p_identificadorProducto;
END$$
DELIMITER ;