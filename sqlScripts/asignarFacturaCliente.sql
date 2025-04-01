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

    SET p_resultado = 0;
    SET p_mensaje = '';
    

    SELECT COUNT(*) INTO cliente_existe FROM CLIENTE WHERE cedulaCliente = p_idCliente;
    
    SELECT COUNT(*) INTO factura_existe FROM FACTURA WHERE identificadorFactura = p_idFactura;
    SELECT COUNT(*) INTO relacion_existe FROM cliente_factura 
    WHERE idCliente = p_idCliente AND idFactura = p_idFactura;

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
        INSERT INTO cliente_factura(idCliente, idFactura)
        VALUES(p_idCliente, p_idFactura);
        
        SET p_resultado = 1;
        SET p_mensaje = 'Factura asociada al cliente correctamente';
    END IF;
END$$

DELIMITER ;