create table usuarioAdmin(
	idUsuario INT auto_increment PRIMARY KEY,
    nombreUsuario VARCHAR(100)
);

DROP TABLE IF EXISTS contraseñaAdmin;
CREATE TABLE contraseñaAdmin(
   idContraseña INT  auto_increment primary key,
    idUsuario INT,
    clave VARCHAR(50),
    foreign key (idUsuario) references usuarioAdmin(idUsuario)
);

insert into usuarioAdmin (nombreUsuario) VALUES ("puntoVenta");
insert into contraseñaAdmin(idUsuario, clave) VALUES (1,"puntoVenta");