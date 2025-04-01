USE bazardb;
CREATE TABLE PERSONA(
	numCedula INT primary key,
    nombre VARCHAR(100),
    apellido VARCHAR(100)
);
CREATE TABLE FAMILIAPRODUCTO(
	identificadorFamilia VARCHAR(100) primary KEY,
    descripcionFamilia VARCHAR(200)
);
CREATE TABLE PRODUCTO(
	identificadorProducto VARCHAR(100) PRIMARY KEY,
    identificadorFamilia VARCHAR(100),
    nombreProducto VARCHAR(100),
    costo DOUBLE,
    precio DOUBLE,
    cantidadDisponible INT,
    
    FOREIGN KEY (identificadorFamilia) REFERENCES FAMILIAPRODUCTO(identificadorFamilia)
    
);

CREATE TABLE COTIZACION(
	numeroCotizacion INT auto_increment PRIMARY KEY,
    estadoCotizacion BOOLEAN
);
CREATE TABLE FACTURA(
	identificadorFactura INT auto_increment PRIMARY KEY,
    fechaFacturado DATE,
    subtotal DOUBLE,
    total DOUBLE
);
CREATE TABLE CLIENTE(
	idCliente INT auto_increment PRIMARY KEY,
    cedulaCliente INT,
    FOREIGN KEY (cedulaCliente) REFERENCES PERSONA(numCedula)
);
CREATE TABLE ADMINISTRADOR(
	idAdministrador INT auto_increment PRIMARY KEY,
    cedulaAdministrador INT,
    FOREIGN KEY (cedulaAdministrador) REFERENCES PERSONA(numCedula)
);

CREATE TABLE USUARIO(
	idUsuario INT auto_increment PRIMARY KEY,
    idAdministrador INT,
    nombreUsuario VARCHAR(100),
    FOREIGN KEY (idAdministrador) references ADMINISTRADOR(idAdministrador)
);
CREATE TABLE CONTRASEÑA(
    idContraseña INT  auto_increment primary key,
    idUsuario INT,
    clave VARCHAR(50),
    foreign key (idUsuario) references USUARIO(idUsuario)
);
CREATE TABLE COTIZACION_PRODUCTO (
    idCotizacion INT,
    identificadorProducto VARCHAR(100),
    cantidad INT,
    PRIMARY KEY (idCotizacion, identificadorProducto),
    FOREIGN KEY (idCotizacion) REFERENCES COTIZACION(numeroCotizacion),
    FOREIGN KEY (identificadorProducto) REFERENCES PRODUCTO(identificadorProducto)
);
CREATE TABLE FACTURA_PRODUCTO (
    idFactura INT,
    identificadorProducto VARCHAR(100),
    cantidad INT,
    precioUnitario DOUBLE,
    PRIMARY KEY (idFactura, identificadorProducto),
    FOREIGN KEY (idFactura) REFERENCES FACTURA(identificadorFactura),
    FOREIGN KEY (identificadorProducto) REFERENCES PRODUCTO(identificadorProducto)
);

CREATE TABLE CLIENTE_FACTURA (
    idCliente INT,
    idFactura INT,
    PRIMARY KEY (idCliente, idFactura),
    FOREIGN KEY (idCliente) REFERENCES CLIENTE(cedulaCliente),
    FOREIGN KEY (idFactura) REFERENCES FACTURA(identificadorFactura)
);




