DELIMITER $$

CREATE PROCEDURE reabajarStock(IN codigoProducto VARCHAR(100), IN cantidadRebajar INT)
BEGIN
    DECLARE cantidadActual INT;
    SELECT cantidadDisponible INTO cantidadActual
    FROM PRODUCTO
    WHERE identificadorProducto = codigoProducto;
    IF cantidadActual < cantidadRebajar THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'No hay suficiente stock para rebajar la cantidad solicitada';
    ELSE
        UPDATE PRODUCTO
        SET cantidadDisponible = cantidadDisponible - cantidadRebajar
        WHERE identificadorProducto = codigoProducto;
        SELECT 'Stock actualizado exitosamente' AS mensaje;
    END IF;
END $$

DELIMITER ;