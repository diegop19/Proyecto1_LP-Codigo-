DELIMITER $$
CREATE PROCEDURE actualizarCantidadProducto(
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidadAumento INT
)
BEGIN
    DECLARE producto_existe INT;
    DECLARE cantidad_actual INT;
    DECLARE nueva_cantidad INT;

    SELECT COUNT(*) INTO producto_existe
    FROM PRODUCTO
    WHERE identificadorProducto = p_identificadorProducto;
    
    IF producto_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'El producto no existe';
    ELSE
        SELECT cantidadDisponible INTO cantidad_actual
        FROM PRODUCTO
        WHERE identificadorProducto = p_identificadorProducto;

        SET nueva_cantidad = cantidad_actual + p_cantidadAumento;
        
        IF nueva_cantidad < 0 THEN
            SET nueva_cantidad = 0;
            SELECT CONCAT(
                'Advertencia: No hay suficiente stock. ',
                'La cantidad disponible se ha ajustado a 0.'
            ) AS mensaje;
        END IF;

        UPDATE PRODUCTO
        SET cantidadDisponible = nueva_cantidad
        WHERE identificadorProducto = p_identificadorProducto;
    END IF;
END $$

DELIMITER ;
