DELIMITER //

CREATE PROCEDURE removerProducto(
    IN identificadorProducto VARCHAR(100)
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    IF NOT EXISTS (SELECT 1 FROM productos WHERE id_producto = identificadorProducto) THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'El producto no existe';
    END IF;
    
    DELETE FROM productos 
    WHERE id_producto = identificadorProducto;
    
    COMMIT;
END //

DELIMITER ;