DELIMITER $$
CREATE PROCEDURE insertar_producto(
    IN p_identificadorProducto VARCHAR(100),
    IN p_identificadorFamilia VARCHAR(100),
    IN p_nombreProducto VARCHAR(100),
    IN p_costo DOUBLE,
    IN p_precio DOUBLE,
    IN p_cantidadDisponible INT
)
BEGIN
    DECLARE familia_existe INT;
    SELECT COUNT(*) INTO familia_existe
    FROM FAMILIAPRODUCTO
    WHERE identificadorFamilia = p_identificadorFamilia;
    IF familia_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'La familia de producto no existe';
    ELSE
        INSERT INTO PRODUCTO (
            identificadorProducto,
            identificadorFamilia,
            nombreProducto,
            costo,
            precio,
            cantidadDisponible
        )
        VALUES (
            p_identificadorProducto,
            p_identificadorFamilia,
            p_nombreProducto,
            p_costo,
            p_precio,
            p_cantidadDisponible
        );
    END IF;
END $$
DELIMITER ;