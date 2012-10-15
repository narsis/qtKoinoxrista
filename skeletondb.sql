SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `koinoxrista` DEFAULT CHARACTER SET utf8 ;
USE `koinoxrista` ;

-- -----------------------------------------------------
-- Table `koinoxrista`.`price_list`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`price_list` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`price_list` (
  `idprice` INT(11) NOT NULL AUTO_INCREMENT ,
  `price` FLOAT NOT NULL ,
  `priceName` VARCHAR(45) NOT NULL DEFAULT '' ,
  `comment` VARCHAR(45) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idprice`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`polikatoikies`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`polikatoikies` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`polikatoikies` (
  `idpolikat` INT(11) NOT NULL AUTO_INCREMENT ,
  `namePolikatoikia` VARCHAR(45) NOT NULL ,
  `address` VARCHAR(100) NOT NULL DEFAULT '' ,
  `synolikoEmbado` DOUBLE NOT NULL DEFAULT '0' ,
  `exeiThermansi` TINYINT(4) NOT NULL DEFAULT '1' ,
  `priceKoinoxriston` INT(11) NOT NULL ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idpolikat`) ,
  UNIQUE INDEX `namePolikatoikia_UNIQUE` (`namePolikatoikia` ASC) ,
  INDEX `fk_polikatoikies_1` (`priceKoinoxriston` ASC) ,
  CONSTRAINT `fk_polikatoikies_1`
    FOREIGN KEY (`priceKoinoxriston` )
    REFERENCES `koinoxrista`.`price_list` (`idprice` )
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`idioktites_enoikiastes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`idioktites_enoikiastes` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`idioktites_enoikiastes` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `onoma` VARCHAR(45) NOT NULL ,
  `eponimo` VARCHAR(45) NOT NULL ,
  `telephone` VARCHAR(15) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`apartments`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`apartments` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`apartments` (
  `idapartments` INT(11) NOT NULL AUTO_INCREMENT ,
  `idpolikat` INT(11) NOT NULL ,
  `orofos` SMALLINT(6) NOT NULL DEFAULT '1' ,
  `embado` DOUBLE NOT NULL DEFAULT '0' ,
  `katoikeitai` TINYINT(4) NOT NULL DEFAULT '1' ,
  `symmetexeiAsanser` TINYINT(4) NOT NULL DEFAULT '1' ,
  `aytonomi8ermansi` TINYINT(4) NOT NULL DEFAULT '0' ,
  `xiliosta` DOUBLE NOT NULL DEFAULT '0' ,
  `idioktitis` INT(11) NOT NULL ,
  `enoikiastis` INT(11) NOT NULL ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idapartments`, `idpolikat`) ,
  INDEX `fk_apartments_1` (`idpolikat` ASC) ,
  INDEX `fk_apartments_2` (`idioktitis` ASC) ,
  INDEX `fk_apartments_3` (`enoikiastis` ASC) ,
  CONSTRAINT `fk_apartments_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_apartments_2`
    FOREIGN KEY (`idioktitis` )
    REFERENCES `koinoxrista`.`idioktites_enoikiastes` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_apartments_3`
    FOREIGN KEY (`enoikiastis` )
    REFERENCES `koinoxrista`.`idioktites_enoikiastes` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



-- -----------------------------------------------------
-- Table `koinoxrista`.`categories`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`categories` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`categories` (
  `idcategories` INT(11) NOT NULL AUTO_INCREMENT ,
  `perigrafy` VARCHAR(45) NOT NULL DEFAULT '' ,
  `comments` VARCHAR(100) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idcategories`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`eidi_ergasion`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`eidi_ergasion` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`eidi_ergasion` (
  `idErgasias` INT(11) NOT NULL ,
  `onomasia` VARCHAR(45) NOT NULL DEFAULT '' ,
  `tropos_diameirasmou` ENUM('xiliosta','embado','apokopi') NOT NULL DEFAULT 'xiliosta' ,
  `kena_eksairountai` TINYINT(4) NOT NULL DEFAULT '0' ,
  `epanalamvanomeni` TINYINT(4) NOT NULL DEFAULT '0' ,
  `einai_kaysima` TINYINT(4) NOT NULL DEFAULT '0' ,
  `comments` VARCHAR(100) NOT NULL DEFAULT '' ,
  `idCaterory` INT(11) NOT NULL ,
  PRIMARY KEY (`idErgasias`) ,
  INDEX `fk_eidi_ergasion_1` (`idCaterory` ASC) ,
  CONSTRAINT `fk_eidi_ergasion_1`
    FOREIGN KEY (`idCaterory` )
    REFERENCES `koinoxrista`.`categories` (`idcategories` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



-- -----------------------------------------------------
-- Table `koinoxrista`.`eidi_logariasmwn`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`eidi_logariasmwn` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`eidi_logariasmwn` (
  `id` INT(11) NOT NULL ,
  `eidos_logariasmou` VARCHAR(45) NOT NULL DEFAULT '' ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  `idCategory` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `eidos_logariasmou_UNIQUE` (`eidos_logariasmou` ASC) ,
  INDEX `fk_eidi_logariasmwn_1` (`idCategory` ASC) ,
  CONSTRAINT `fk_eidi_logariasmwn_1`
    FOREIGN KEY (`idCategory` )
    REFERENCES `koinoxrista`.`categories` (`idcategories` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



-- -----------------------------------------------------
-- Table `koinoxrista`.`fuel_tank`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`fuel_tank` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`fuel_tank` (
  `idtank` INT(11) NOT NULL AUTO_INCREMENT ,
  `idpolikat` INT(11) NOT NULL ,
  `capacity` BIGINT(20) NOT NULL DEFAULT '0' COMMENT 'Χωρητικότητα σε λίτρα' ,
  `warning_threshold` SMALLINT(6) NOT NULL DEFAULT '20' ,
  PRIMARY KEY (`idtank`) ,
  UNIQUE INDEX `idpolikat_UNIQUE` (`idpolikat` ASC) ,
  INDEX `fk_fuel_tank_1` (`idpolikat` ASC) ,
  CONSTRAINT `fk_fuel_tank_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



-- -----------------------------------------------------
-- Table `koinoxrista`.`koinoxrista`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`koinoxrista` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`koinoxrista` (
  `idpolikat` INT(11) NOT NULL ,
  `idApartment` INT(11) NOT NULL ,
  `idCategory` INT(11) NOT NULL ,
  `date` DATE NOT NULL DEFAULT '2000-01-01' ,
  `poso` FLOAT NOT NULL DEFAULT '0' ,
  `comments` VARCHAR(100) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idpolikat`, `idApartment`, `idCategory`, `date`) ,
  INDEX `fk_koinoxrista_1` (`idpolikat` ASC) ,
  INDEX `fk_koinoxrista_2` (`idApartment` ASC) ,
  INDEX `fk_koinoxrista_3` (`idCategory` ASC) ,
  CONSTRAINT `fk_koinoxrista_3`
    FOREIGN KEY (`idCategory` )
    REFERENCES `koinoxrista`.`categories` (`idcategories` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_koinoxrista_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_koinoxrista_2`
    FOREIGN KEY (`idApartment` )
    REFERENCES `koinoxrista`.`apartments` (`idapartments` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`logariasmoi_polikatoikiwn`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`logariasmoi_polikatoikiwn` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`logariasmoi_polikatoikiwn` (
  `id` INT(11) NOT NULL ,
  `idpolikat` INT(11) NOT NULL ,
  `hmerominia_pliromis` DATE NOT NULL DEFAULT '2000-01-01' ,
  `poso_pliromis` FLOAT NOT NULL DEFAULT '0' ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`id`, `idpolikat`, `hmerominia_pliromis`) ,
  INDEX `fk_logariasmoi_polikatoikiwn_1` (`idpolikat` ASC) ,
  INDEX `fk_logariasmoi_polikatoikiwn_2` (`id` ASC) ,
  CONSTRAINT `fk_logariasmoi_polikatoikiwn_2`
    FOREIGN KEY (`id` )
    REFERENCES `koinoxrista`.`eidi_logariasmwn` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_logariasmoi_polikatoikiwn_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`pliromes_koinoxrista`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`pliromes_koinoxrista` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`pliromes_koinoxrista` (
  `idpliromes_koinoxrista` INT(11) NOT NULL ,
  `date_pliromis` DATE NULL DEFAULT NULL ,
  `idapartment` INT(11) NOT NULL ,
  `idpolikat` INT(11) NOT NULL ,
  `poso` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`idpliromes_koinoxrista`) ,
  INDEX `fk_pliromes_koinoxrista_1` (`idapartment` ASC) ,
  INDEX `fk_pliromes_koinoxrista_2` (`idpolikat` ASC) ,
  CONSTRAINT `fk_pliromes_koinoxrista_1`
    FOREIGN KEY (`idapartment` )
    REFERENCES `koinoxrista`.`apartments` (`idapartments` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_pliromes_koinoxrista_2`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`apartments` (`idpolikat` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`pliromes_logariasmwn`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`pliromes_logariasmwn` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`pliromes_logariasmwn` (
  `id` INT(11) NOT NULL ,
  `idpolikat` INT(11) NOT NULL ,
  `hmerominia_pliromis` DATE NOT NULL ,
  `hmerominia_dienergias` DATE NOT NULL DEFAULT '2000-01-01' ,
  `comments` VARCHAR(100) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`id`, `idpolikat`, `hmerominia_pliromis`) ,
  INDEX `fk_pliromes_logariasmwn_1` (`id` ASC) ,
  INDEX `fk_pliromes_logariasmwn_2` (`idpolikat` ASC) ,
  CONSTRAINT `fk_pliromes_logariasmwn_1`
    FOREIGN KEY (`id` )
    REFERENCES `koinoxrista`.`logariasmoi_polikatoikiwn` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_pliromes_logariasmwn_2`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`logariasmoi_polikatoikiwn` (`idpolikat` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`texnites_promitheftes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`texnites_promitheftes` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`texnites_promitheftes` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `eponimia` VARCHAR(80) NOT NULL DEFAULT '' ,
  `address` VARCHAR(80) NOT NULL DEFAULT '' ,
  `tilefono` VARCHAR(15) NOT NULL DEFAULT '' ,
  `email` VARCHAR(45) NOT NULL DEFAULT '' ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



-- -----------------------------------------------------
-- Table `koinoxrista`.`prosfores_timwn`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`prosfores_timwn` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`prosfores_timwn` (
  `idPolikat` INT(11) NOT NULL ,
  `idTexnikou` INT(11) NOT NULL ,
  `idErgasias` INT(11) NOT NULL ,
  `hmerominiaProsforas` DATE NOT NULL DEFAULT '2000-01-01' ,
  `litraKaysimou` INT(11) NOT NULL DEFAULT '0' ,
  `timi` FLOAT NOT NULL DEFAULT '0' ,
  `comment` VARCHAR(100) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idPolikat`, `idTexnikou`, `idErgasias`, `hmerominiaProsforas`) ,
  INDEX `fk_prosfores_timwn_1` (`idPolikat` ASC) ,
  INDEX `fk_prosfores_timwn_2` (`idTexnikou` ASC) ,
  INDEX `fk_prosfores_timwn_3` (`idErgasias` ASC) ,
  CONSTRAINT `fk_prosfores_timwn_1`
    FOREIGN KEY (`idPolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_prosfores_timwn_2`
    FOREIGN KEY (`idTexnikou` )
    REFERENCES `koinoxrista`.`texnites_promitheftes` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_prosfores_timwn_3`
    FOREIGN KEY (`idErgasias` )
    REFERENCES `koinoxrista`.`eidi_ergasion` (`idErgasias` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`pliromes_texnikwn`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`pliromes_texnikwn` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`pliromes_texnikwn` (
  `idpolikat` INT(11) NOT NULL ,
  `idTexnikou` INT(11) NOT NULL ,
  `idErgasias` INT(11) NOT NULL ,
  `date_offered` DATE NOT NULL DEFAULT '2000-01-01' ,
  `date_transaction` DATE NOT NULL DEFAULT '2000-01-01' ,
  `comments` VARCHAR(100) NOT NULL DEFAULT '' ,
  PRIMARY KEY (`idpolikat`, `idTexnikou`, `idErgasias`, `date_offered`) ,
  INDEX `fk_pliromes_texnikwn_1` (`idpolikat` ASC) ,
  INDEX `fk_pliromes_texnikwn_2` (`idTexnikou` ASC) ,
  INDEX `fk_pliromes_texnikwn_3` (`idErgasias` ASC) ,
  CONSTRAINT `fk_pliromes_texnikwn_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`prosfores_timwn` (`idPolikat` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_pliromes_texnikwn_2`
    FOREIGN KEY (`idTexnikou` )
    REFERENCES `koinoxrista`.`prosfores_timwn` (`idTexnikou` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_pliromes_texnikwn_3`
    FOREIGN KEY (`idErgasias` )
    REFERENCES `koinoxrista`.`prosfores_timwn` (`idErgasias` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`texnites_kanoun_ergasies`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`texnites_kanoun_ergasies` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`texnites_kanoun_ergasies` (
  `id` INT(11) NOT NULL ,
  `idErgasias` INT(11) NOT NULL ,
  PRIMARY KEY (`id`, `idErgasias`) ,
  INDEX `fk_texnites_kanoun_ergasies_1` (`id` ASC) ,
  INDEX `fk_texnites_kanoun_ergasies_2` (`idErgasias` ASC) ,
  CONSTRAINT `fk_texnites_kanoun_ergasies_2`
    FOREIGN KEY (`idErgasias` )
    REFERENCES `koinoxrista`.`eidi_ergasion` (`idErgasias` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_texnites_kanoun_ergasies_1`
    FOREIGN KEY (`id` )
    REFERENCES `koinoxrista`.`texnites_promitheftes` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`users`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`users` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`users` (
  `idusers` INT(11) NOT NULL AUTO_INCREMENT ,
  `username` VARCHAR(45) NOT NULL ,
  `password` VARCHAR(45) NOT NULL DEFAULT '' ,
  `comments` VARCHAR(200) NOT NULL DEFAULT '' ,
  `isAdmin` TINYINT(4) NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`idusers`) ,
  UNIQUE INDEX `username_UNIQUE` (`username` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `koinoxrista`.`userAccess`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `koinoxrista`.`userAccess` ;

CREATE  TABLE IF NOT EXISTS `koinoxrista`.`userAccess` (
  `idUser` INT(11) NOT NULL ,
  `idpolikat` INT(11) NOT NULL ,
  PRIMARY KEY (`idUser`, `idpolikat`) ,
  INDEX `fk_userAccess_1` (`idpolikat` ASC) ,
  INDEX `fk_userAccess_2` (`idUser` ASC) ,
  CONSTRAINT `fk_userAccess_1`
    FOREIGN KEY (`idpolikat` )
    REFERENCES `koinoxrista`.`polikatoikies` (`idpolikat` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_userAccess_2`
    FOREIGN KEY (`idUser` )
    REFERENCES `koinoxrista`.`users` (`idusers` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
