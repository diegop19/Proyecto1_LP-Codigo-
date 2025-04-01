DROP PROCEDURE IF EXISTS agregarFacturaCliente;
DELIMITER $$

CREATE PROCEDURE agregarFacturaCliente(
    IN p_idCliente INT,
    IN p_idFactura INT,
    OUT p_resultado INT,
    OUT p_mensaje VARCHAR(255))
BEGIN
    DECLARE cliente_existe INT;
    DECLARE factura_existe INT;
    DECLARE relacion_existe INT;
    
    -- Inicializar variables de salida
    SET p_resultado = 0;
    SET p_mensaje = '';
    
    -- Verificar si el cliente existe
    SELECT COUNT(*) INTO cliente_existe FROM CLIENTE WHERE cedulaCliente = p_idCliente;
    
    -- Verificar si la factura existe (asumiendo que hay una tabla FACTURA)
    SELECT COUNT(*) INTO factura_existe FROM FACTURA WHERE identificadorFactura = p_idFactura;
    
    -- Verificar si ya existe esta relación
    SELECT COUNT(*) INTO relacion_existe FROM cliente_factura 
    WHERE idCliente = p_idCliente AND idFactura = p_idFactura;
    
    -- Validaciones
    IF cliente_existe = 0 THEN
        SET p_resultado = -1;
        SET p_mensaje = 'Error: El cliente no existe';
    ELSEIF factura_existe = 0 THEN
        SET p_resultado = -2;
        SET p_mensaje = 'Error: La factura no existe';
    ELSEIF relacion_existe > 0 THEN
        SET p_resultado = -3;
        SET p_mensaje = 'Error: Esta factura ya está asociada al cliente';
    ELSE
        -- Insertar la relación
        INSERT INTO cliente_factura(idCliente, idFactura)
        VALUES(p_idCliente, p_idFactura);
        
        SET p_resultado = 1;
        SET p_mensaje = 'Factura asociada al cliente correctamente';
    END IF;
END$$

DELIMITER ;