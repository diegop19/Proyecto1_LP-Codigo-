USE bazardb;
DROP PROCEDURE IF EXISTS insertarFamilia;
DELIMITER $$

CREATE PROCEDURE insertarFamilia(
	IN p_identificadorFamilia VARCHAR(100),
    IN p_descripcionFamilia VARCHAR(100)
)
BEGIN
	INSERT INTO familiaproducto(identificadorFamilia, descripcionFamilia)
    VALUES (p_identificadorFamilia, p_descripcionFamilia);
END $$

DELIMITER ;