CREATE TABLE GAMES (
    game_id INT AUTO_INCREMENT PRIMARY KEY,
    game_type VARCHAR(50),
    players INT,
    moves INT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE MOVES (
    game_id INT NOT NULL,
    move_id INT NOT NULL,
    x1 INT,
    y1 INT,
    x2 INT,
    y2 INT,
    PRIMARY KEY (game_id, move_id),
    FOREIGN KEY (game_id) REFERENCES GAMES(game_id)
);

DELIMITER $$

--CREATE TRIGGER MOVES_INSERT_ADD
--AFTER INSERT ON MOVES
--FOR EACH ROW
--BEGIN
--    UPDATE GAMES
--    SET move_id = move_id + 1
--    WHERE game_id = NEW.game_id;
--END$$
--DELIMITER ;
