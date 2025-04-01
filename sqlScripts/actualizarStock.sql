DELIMITER $$

CREATE PROCEDURE reabajarStock(IN codigoProducto VARCHAR(100), IN cantidadRebajar INT)
BEGIN
    UPDATE PRODUCTO
    SET cantidadDisponible = cantidadDisponible - cantidadRebajar
    WHERE identificadorProducto = codigoProducto;
    IF ROW_COUNT() = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Producto no encontrado';
    ELSE
        SELECT 'Stock actualizado exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;