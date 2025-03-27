USE bazardb;
DROP PROCEDURE IF EXISTS filtrarProductos;
DELIMITER $$

CREATE PROCEDURE filtrarProductos(
    IN p_identificadorFamilia VARCHAR(100))
BEGIN
    -- Verificar si la familia existe
    DECLARE familia_existe INT;
    SELECT COUNT(*) INTO familia_existe
    FROM FAMILIAPRODUCTO
    WHERE identificadorFamilia = p_identificadorFamilia;
    
    IF familia_existe = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'La familia especificada no existe';
    ELSE
        SELECT 
            p.identificadorProducto,
            p.nombreProducto,
            p.costo,
            p.precio,
            p.cantidadDisponible,
            fp.identificadorFamilia
        FROM 
            PRODUCTO p
        JOIN 
            FAMILIAPRODUCTO fp ON p.identificadorFamilia = fp.identificadorFamilia
        WHERE 
            p.identificadorFamilia = p_identificadorFamilia
        ORDER BY 
            p.nombreProducto;
    END IF;
END $$

DELIMITER ;