DELIMITER $$
CREATE PROCEDURE actualizarCantidadProducto(
    IN p_identificadorProducto VARCHAR(100),
    IN p_cantidadAumento INT
)
BEGIN
    DECLARE producto_existe INT;
    SELECT COUNT(*) INTO producto_existe
    FROM PRODUCTO
    WHERE identificadorProducto = p_identificadorProducto;
    IF producto_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'El producto no existe';
    ELSE
        UPDATE PRODUCTO
        SET cantidadDisponible = cantidadDisponible + p_cantidadAumento
        WHERE identificadorProducto = p_identificadorProducto;
    END IF;
END $$

DELIMITER ;
