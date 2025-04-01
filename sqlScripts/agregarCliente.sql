DELIMITER $$

CREATE PROCEDURE agregarCliente(
    IN p_numCedula INT,
    IN p_nombre VARCHAR(100),
    IN p_apellido VARCHAR(100),
    OUT p_resultado INT,
    OUT p_mensaje VARCHAR(255))
BEGIN
    DECLARE persona_existe INT;
    DECLARE cliente_existe INT;
    
    SET p_resultado = 0;
    SET p_mensaje = '';

    SELECT COUNT(*) INTO persona_existe FROM PERSONA WHERE numCedula = p_numCedula;

    IF persona_existe > 0 THEN
        SELECT COUNT(*) INTO cliente_existe FROM CLIENTE WHERE cedulaCliente = p_numCedula;
    ELSE
        SET cliente_existe = 0;
    END IF;

    IF cliente_existe > 0 THEN
        SET p_resultado = -1;
        SET p_mensaje = 'Error: La persona ya está registrada como cliente';
    ELSE

        IF persona_existe = 0 THEN
            INSERT INTO PERSONA(numCedula, nombre, apellido)
            VALUES(p_numCedula, p_nombre, p_apellido);
            
            SET p_mensaje = CONCAT(p_mensaje, 'Persona agregada. ');
        END IF;

        INSERT INTO CLIENTE(cedulaCliente)
        VALUES(p_numCedula);
        
        SET p_resultado = 1;
        SET p_mensaje = CONCAT(p_mensaje, 'Cliente registrado exitosamente');
    END IF;
END$$

DELIMITER ;