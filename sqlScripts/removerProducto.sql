USE bazardb;
DROP PROCEDURE IF EXISTS REMOVERPRODUCTO;
DELIMITER //

CREATE PROCEDURE removerProducto(
    IN p_identificadorProducto VARCHAR(100)
)
BEGIN
    DELETE FROM producto
    WHERE identificadorProducto = p_identificadorProducto;
END //

DELIMITER ;