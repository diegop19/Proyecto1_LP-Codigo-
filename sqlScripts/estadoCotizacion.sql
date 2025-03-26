DELIMITER $$

CREATE PROCEDURE obtenerEstadoCotizacion(IN numCotizacion INT)
BEGIN
    SELECT estadoCotizacion
    FROM COTIZACION
    WHERE numeroCotizacion = numCotizacion;
END $$

DELIMITER ;
