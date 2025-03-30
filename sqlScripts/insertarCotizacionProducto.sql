DELIMITER $$

CREATE PROCEDURE insertarCotizacionProducto(
    IN p_idCotizacion INT,
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidad INT
)
BEGIN
    INSERT INTO COTIZACION_PRODUCTO (idCotizacion, identificadorProducto, cantidad)
    VALUES (p_idCotizacion, p_identificadorProducto, p_cantidad);
END $$

DELIMITER ;