DELIMITER $$
CREATE PROCEDURE actualizarEstadoCotizacion(
    IN numCotizacion INT, 
    OUT resultado INT
)
BEGIN
    DECLARE estadoActual BOOLEAN;

    SELECT estadoCotizacion INTO estadoActual
    FROM COTIZACION
    WHERE numeroCotizacion = numCotizacion;
    
    IF estadoActual IS NULL THEN
        SET resultado = -1;
    ELSE
        IF estadoActual = 0 THEN
            UPDATE COTIZACION 
            SET estadoCotizacion = 1 
            WHERE numeroCotizacion = numCotizacion;
            
            SET resultado = 0;
        ELSE
            SET resultado = 1;
        END IF;
    END IF;
END $$
DELIMITER ;