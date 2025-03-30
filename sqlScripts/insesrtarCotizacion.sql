DELIMITER $$
CREATE PROCEDURE insertarCotizacion(OUT p_idCotizacion INT)
BEGIN
INSERT INTO cotizacion(estadoCotizacion) VALUES (0);
SET p_idCotizacion = LAST_INSERT_ID();
END $$
DELIMITER ;
