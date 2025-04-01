DELIMITER $$

CREATE PROCEDURE validarCredencialesAdmin(
    IN p_nombreUsuario VARCHAR(100),
    IN p_clave VARCHAR(50),
    OUT p_resultado INT)
BEGIN
    DECLARE v_usuario_id INT;
    DECLARE v_clave_correcta VARCHAR(50);

    SELECT idUsuario INTO v_usuario_id 
    FROM usuarioAdmin 
    WHERE nombreUsuario = p_nombreUsuario;

    IF v_usuario_id IS NOT NULL THEN
        SELECT clave INTO v_clave_correcta
        FROM contraseñaAdmin
        WHERE idUsuario = v_usuario_id;
        
        IF v_clave_correcta = p_clave THEN
            SET p_resultado = 1; 
        ELSE
            SET p_resultado = 0; 
        END IF;
    ELSE
        SET p_resultado = 0; 
    END IF;
END$$

DELIMITER ;