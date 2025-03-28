USE bazardb;
DROP PROCEDURE IF EXISTS actualizarCantidadProducto;
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
			UPDATE PRODUCTO
            SET cantidadDisponible = 0
            WHERE identificadorProducto = p_identificadorProducto;
			SIGNAL SQLSTATE '01000'  -- Código para warning (no error)
            SET MESSAGE_TEXT = 'Stock insuficiente. Se ajustó a 0.';

       ELSE
			UPDATE PRODUCTO
			SET cantidadDisponible = nueva_cantidad
			WHERE identificadorProducto = p_identificadorProducto;
		END IF;
    END IF;
END $$

DELIMITER ;
