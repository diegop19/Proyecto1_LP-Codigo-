USE bazardb;
DELIMITER $$
CREATE PROCEDURE ObtenerProductoIndividual(IN idProducto VARCHAR(100))
BEGIN
    SELECT nombreProducto, precio 
    FROM PRODUCTO
    WHERE identificadorProducto = idProducto;
END $$
DELIMITER ;