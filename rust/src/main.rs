#[derive(Clone, Copy, PartialEq)]
enum Player {
    White,
    Black,
}
impl Player {
    const fn next(self) -> Self {
        match self {
            Self::White => Self::Black,
            Self::Black => Self::White,
        }
    }
}
impl std::fmt::Display for Player {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Self::White => write!(f, "White"),
            Self::Black => write!(f, "Black"),
        }
    }
}

enum Winner {
    None,
    Player(Player),
    Draw,
}
impl Winner {
    fn add(&mut self, player: Player) {
        match self {
            Self::None => *self = Self::Player(player),
            Self::Player(p) => {
                if *p != player {
                    *self = Self::Draw;
                }
            }
            Self::Draw => (),
        }
    }
}

struct Pentago {
    board: [[Option<Player>; 9]; 4],
    turn: Player,
}
impl Pentago {
    const fn new() -> Self {
        Self {
            board: [[None; 9]; 4],
            turn: Player::White,
        }
    }
    fn play(&mut self) {
        println!("{self}");
        loop {
            self.place_piece();
            if self.game_over() {
                break;
            }
            self.rotate_sector();
            if self.game_over() {
                break;
            }
            self.turn = self.turn.next();
        }
    }
    fn place_piece(&mut self) {
        loop {
            let mut input = String::new();
            clear_screen();
            println!("{self}");
            println!("{turn}'s turn", turn = self.turn);
            println!("Location (ie. A4 or C9): ");
            std::io::stdin()
                .read_line(&mut input)
                .expect("Failed to read line");
            let input = input.trim();
            let input: Vec<char> = input.chars().collect();
            if input.len() != 2 {
                continue;
            }
            let sector = match input.get(0) {
                Some('A'..='D') => input[0] as usize - 'A' as usize,
                _ => continue,
            };
            let position = match input.get(1) {
                Some('1'..='9') => input[1] as usize - '1' as usize,
                _ => continue,
            };
            self.board[sector][position] = match self.board[sector][position] {
                None => Some(self.turn),
                _ => continue,
            };
            break;
        }
    }
    fn rotate_sector(&mut self) {
        loop {
            let mut input = String::new();
            clear_screen();
            println!("{self}");
            println!("{turn}'s turn", turn = self.turn);
            println!("Rotation (ie. A' or C\"): ");
            std::io::stdin()
                .read_line(&mut input)
                .expect("Failed to read line");
            let input = input.trim();
            let input: Vec<char> = input.chars().collect();
            if input.len() != 2 {
                continue;
            }
            let sector = match input.get(0) {
                Some('A'..='D') => input[0] as usize - 'A' as usize,
                _ => continue,
            };
            let direction = match input.get(1) {
                Some('\'' | '"') => input[1],
                _ => continue,
            };
            let mut new_sector = [None; 9];
            for i in 0..3 {
                for j in 0..3 {
                    match direction {
                        '\'' => new_sector[i * 3 + j] = self.board[sector][6 - j * 3 + i],
                        '"' => new_sector[i * 3 + j] = self.board[sector][j * 3 + 2 - i],
                        _ => unreachable!(),
                    }
                }
            }
            self.board[sector] = new_sector;
            break;
        }
    }
    fn game_over(&self) -> bool {
        let mut winner = Winner::None;
        let winning_positions = [
            [0, 3, 6, 18, 21, 24],
            [1, 4, 7, 19, 22, 25],
            [2, 5, 8, 20, 23, 26],
            [9, 12, 15, 27, 30, 33],
            [10, 13, 16, 28, 31, 34],
            [11, 14, 17, 29, 32, 35],
            // rows
            [0, 1, 2, 9, 10, 11],
            [3, 4, 5, 12, 13, 14],
            [6, 7, 8, 15, 16, 17],
            [18, 19, 20, 27, 28, 29],
            [21, 22, 23, 30, 31, 32],
            [24, 25, 26, 33, 34, 35],
            // diagonals
            [0, 4, 8, 27, 31, 35],
            [11, 13, 15, 20, 22, 24],
            [10, 12, 8, 19, 21, 36],
            [14, 16, 27, 23, 25, 36],
            [1, 5, 15, 28, 31, 36],
            [3, 7, 20, 30, 34, 36],
        ];
        for position in &winning_positions {
            for i in position.windows(5) {
                if i.contains(&36) {
                    continue;
                }
                let mut player = None;
                let mut count = 0;
                for j in i {
                    if player.is_none() {
                        player = self.board[*j / 9][*j % 9];
                        count = 1;
                    }
                    if player == self.board[*j / 9][*j % 9] {
                        count += 1;
                    }
                }
                if player.is_some() && count == 5 {
                    winner.add(player.unwrap());
                }
            }
        }
        match winner {
            Winner::None => false,
            Winner::Player(player) => {
                clear_screen();
                println!("{self}");
                println!("{player} wins!");
                true
            }
            Winner::Draw => {
                clear_screen();
                println!("{self}");
                println!("Draw!");
                true
            }
        }
    }
}

impl std::fmt::Display for Pentago {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        let mut s = String::new();
        for i in 0..2 {
            for j in 0..3 {
                for k in 0..2 {
                    for l in 0..3 {
                        let letter = match self.board[i + k * 2][j * 3 + l] {
                            None => " ",
                            Some(Player::White) => "W",
                            Some(Player::Black) => "B",
                        };
                        s.push_str(&format!("{letter} "));
                    }
                    match k {
                        0 => s.push_str("| "),
                        _ => s.push('\n'),
                    }
                }
            }
            if i == 0 {
                s.push_str("------+------\n");
            }
        }
        write!(f, "{s}")
    }
}

fn clear_screen() {
    print!("\x1B[2J");
}
fn main() {
    let mut game = Pentago::new();
    game.play();
}
