DELIMITER $$

CREATE PROCEDURE obtenerDisponible(IN idProducto VARCHAR(100), OUT cantidadEnStock INT)
BEGIN

    SELECT cantidadDisponible
    INTO cantidadEnStock
    FROM PRODUCTO
    WHERE identificadorProducto = idProducto;

    
    IF cantidadEnStock IS NULL THEN
        SET cantidadEnStock = 0;
    END IF;
END $$

DELIMITER ;