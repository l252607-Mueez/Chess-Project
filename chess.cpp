#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;


// to draw the chessboard
void drawBoard(RenderWindow& window, int boxSize,
	RectangleShape& lightBox,
	RectangleShape& darkBox,
	Color normalLight, Color normalDark, bool highlight[][8],
	Color highlightLight, Color highlightDark,
	Text& label, int blackKingRow, int blackKingCol, int whiteKingRow, int whiteKingCol,
	bool blackCheck, bool whiteCheck, bool gameOver,
	Color checkColor, Color checkmateColor) {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bool isLight = ((i + j) % 2 == 0);
			int xPos = j * boxSize;
			int yPos = i * boxSize;
			Color currentColor;

			if (gameOver) {
				// Checkmate highlighting (only if the king on that square is in check)
				if ((blackKingRow == i && blackKingCol == j && blackCheck) ||
					(whiteKingRow == i && whiteKingCol == j && whiteCheck)) {
					currentColor = checkmateColor;
				}
				else if (isLight) {
					currentColor = normalLight;
				}
				else {
					currentColor = normalDark;
				}
			}
			else if ((blackKingRow == i && blackKingCol == j && blackCheck) ||
				(whiteKingRow == i && whiteKingCol == j && whiteCheck)) {
				currentColor = checkColor;
			}
			else if (highlight[i][j]) {
				// simple highlighting on moving pieces
				if (isLight) {
					currentColor = highlightLight;
				}
				else {
					currentColor = highlightDark;
				}
			}
			else {
				// Normal board color
				if (isLight) {
					currentColor = normalLight;
				}
				else {
					currentColor = normalDark;
				}
			}

			// Draw light boxes
			if (isLight) {
				lightBox.setFillColor(currentColor);
				lightBox.setPosition(xPos, yPos);
				window.draw(lightBox);
				label.setFillColor(Color(118, 150, 86));
			}
			// Draw dark boxes
			else {
				darkBox.setFillColor(currentColor);
				darkBox.setPosition(xPos, yPos);
				window.draw(darkBox);
				label.setFillColor(Color(238, 238, 210));
			}

			// Label 1st column (row numbers)
			if (j == 0) {
				label.setString(to_string(8 - i));
				label.setPosition(xPos + (boxSize / 15), yPos + (boxSize / 25));
				window.draw(label);
			}
			// Label 8th row (column letters)
			if (i == 7) {
				label.setString(char(65 + j));
				label.setPosition(xPos + boxSize - (boxSize / 5), yPos + boxSize - (boxSize / 4));
				window.draw(label);
			}
		}
	}
}


// make chessboard for console using board array
void initializeBoard(char board[][8]) {
	// empty board
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = '.';

	// black pieces
	board[0][0] = board[0][7] = 'r'; // rooks
	board[0][1] = board[0][6] = 'n'; // knights
	board[0][2] = board[0][5] = 'b'; // bishops
	board[0][3] = 'q'; // queen
	board[0][4] = 'k'; // king
	for (int j = 0; j < 8; j++)
		board[1][j] = 'p'; // pawns

	// white pieces
	board[7][0] = board[7][7] = 'R'; // rooks
	board[7][1] = board[7][6] = 'N'; // knights
	board[7][2] = board[7][5] = 'B'; // bishops
	board[7][3] = 'Q'; // queen
	board[7][4] = 'K'; // king
	for (int j = 0; j < 8; j++)
		board[6][j] = 'P'; // pawns
}

// print chessboard (board array) in console
void printBoard(char board[][8]) {
	cout << "    A B C D E F G H" << endl; // Column labels
	cout << "   -----------------" << endl;
	for (int i = 0; i < 8; i++) {
		cout << 8 - i << " | "; // Row label
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << " ";
		}
		cout << '|' << endl;
	}
	cout << "   -----------------" << endl;
}


bool isValidMove(int oldRow, int oldCol, int newRow, int newCol, char board[][8]) {

	char piece = board[oldRow][oldCol];

	// Rook
	if (piece == 'r' || piece == 'R') {
		if (oldRow != newRow && oldCol != newCol) {
			return false;
		}
		else if (oldRow == newRow) {
			if (oldCol < newCol) {
				for (int i = oldCol + 1; i < newCol; i++) {
					if (board[oldRow][i] != '.')
						return false;
				}
			}
			else if (oldCol > newCol) {
				for (int i = oldCol - 1; i > newCol; i--) {
					if (board[oldRow][i] != '.')
						return false;
				}
			}
			return true;
		}
		else { // when oldCOl == newCol
			if (oldRow < newRow) {
				for (int i = oldRow + 1; i < newRow; i++) {
					if (board[i][oldCol] != '.')
						return false;
				}
			}
			else if (oldRow > newRow) {
				for (int i = oldRow - 1; i > newRow; i--) {
					if (board[i][oldCol] != '.')
						return false;
				}
			}
			return true;
		}

	}

	// Pawn
	else if (piece == 'P') { // white pawn
		if ((oldRow > newRow)) {
			if (oldCol == newCol) {
				if ((newRow == oldRow - 1) && (board[newRow][newCol] == '.'))
					return true;
				else if ((newRow == oldRow - 2) && (oldRow == 6) && (board[oldRow - 1][oldCol] == '.') && board[newRow][newCol] == '.')
					return true;
			}
			else if ((newRow == oldRow - 1) && ((newCol == oldCol + 1) || (newCol == oldCol - 1)) && (board[newRow][newCol] > 'a' && board[newRow][newCol] < 'z')) {
				return true;
			}
			return false;
		}
		return false;
	}
	else if (piece == 'p') { // black pawn
		if ((oldRow < newRow)) {
			if (oldCol == newCol) {
				if ((newRow == oldRow + 1) && (board[newRow][newCol] == '.'))
					return true;
				else if ((newRow == oldRow + 2) && (oldRow == 1) && (board[oldRow + 1][oldCol] == '.') && board[newRow][newCol] == '.')
					return true;
			}
			else if ((newRow == oldRow + 1) && ((newCol == oldCol + 1) || (newCol == oldCol - 1)) && (board[newRow][newCol] > 'A' && board[newRow][newCol] < 'Z')) {
				return true;
			}
			return false;
		}
		return false;
	}


	// Bishup
	else if (piece == 'b' || piece == 'B') {
		int rowDiff = abs(oldRow - newRow);
		int colDiff = abs(oldCol - newCol);

		if (rowDiff != colDiff) return false;

		int rowStep = -1, colStep = -1;

		if (newRow > oldRow)
			rowStep = 1;
		if (newCol > oldCol) {
			colStep = 1;
		}

		int r = oldRow + rowStep;
		int c = oldCol + colStep;

		while (r != newRow && c != newCol) {
			if (board[r][c] != '.')
				return false;
			r += rowStep;
			c += colStep;
		}

		if (board[newRow][newCol] == '.') return true;

		if (piece == 'B') { // white bishop
			if (board[newRow][newCol] > 'a' && board[newRow][newCol] < 'z')
				return true;
			else
				return false;
		}
		else { // piece == 'b' means black bishop
			if (board[newRow][newCol] > 'A' && board[newRow][newCol] < 'Z')
				return true;
			else
				return false;
		}

	}


	// Knight
	if (piece == 'n' || piece == 'N') {
		int rowDiff = abs(newRow - oldRow);
		int colDiff = abs(newCol - oldCol);

		bool validMove = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);

		if (!validMove)
			return false;

		return true;
	}


	// King
	if (piece == 'k' || piece == 'K') {
		int rowDiff = abs(newRow - oldRow);
		int colDiff = abs(newCol - oldCol);

		if ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1) || (rowDiff == 1 && colDiff == 1))
			return true;

		return false;
	}


	// Queen
	if (piece == 'q' || piece == 'Q') {
		if (oldRow == newRow) {
			if (oldCol < newCol) {
				for (int i = oldCol + 1; i < newCol; i++) {
					if (board[oldRow][i] != '.')
						return false;
				}
			}
			else if (oldCol > newCol) {
				for (int i = oldCol - 1; i > newCol; i--) {
					if (board[oldRow][i] != '.')
						return false;
				}
			}
			return true;
		}
		else if (oldCol == newCol) { // when oldCOl == newCol
			if (oldRow < newRow) {
				for (int i = oldRow + 1; i < newRow; i++) {
					if (board[i][oldCol] != '.')
						return false;
				}
			}
			else if (oldRow > newRow) {
				for (int i = oldRow - 1; i > newRow; i--) {
					if (board[i][oldCol] != '.')
						return false;
				}
			}
		}
		else {
			int rowDiff = abs(oldRow - newRow);
			int colDiff = abs(oldCol - newCol);
			if (rowDiff != colDiff) return false;
			int rowStep = -1, colStep = -1;
			if (newRow > oldRow)
				rowStep = 1;
			if (newCol > oldCol) {
				colStep = 1;
			}
			int r = oldRow + rowStep;
			int c = oldCol + colStep;
			while (r != newRow && c != newCol) {
				if (board[r][c] != '.')
					return false;
				r += rowStep;
				c += colStep;
			}
		}

		return true;
	}
}

void findKingPosition(char board[][8], int& blackKingRow, int& blackKingCol, int& whiteKingRow, int& whiteKingCol) {
	char blackKingChar = 'k';
	char whiteKingChar = 'K';
	bool blackKingFound = false;
	bool whiteKingFound = false;

	// finding the king's position
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (board[r][c] == blackKingChar) {
				blackKingRow = r;
				blackKingCol = c;
				blackKingFound = true;
			}
			else if (board[r][c] == whiteKingChar) {
				whiteKingRow = r;
				whiteKingCol = c;
				whiteKingFound = true;
			}
		}
		if (blackKingFound && whiteKingFound) break;
	}
}

// Check
void isKingAttacked(char board[][8], int blackKingRow, int blackKingCol, int whiteKingRow, int whiteKingCol, bool& blackCheck, bool& whiteCheck) {
	blackCheck = false;
	whiteCheck = false;

	// check if king's position is under attack
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char p = board[i][j];

			if (p >= 'A' && p <= 'Z') { // white piece
				if (isValidMove(i, j, blackKingRow, blackKingCol, board)) {
					blackCheck = true;
				}
			}
			else if (p >= 'a' && p <= 'z') { // black piece
				if (isValidMove(i, j, whiteKingRow, whiteKingCol, board)) {
					whiteCheck = true;
				}
			}
			if (blackCheck && whiteCheck) break;
		}
		if (blackCheck && whiteCheck) break;
	}
}


bool isCheckmate(char board[][8], int kingRow, int kingCol) {
	bool isBlackChecked = false;
	if (board[kingRow][kingCol] == 'k')
		isBlackChecked = true;

	// Check all king moves
	for (int r = -1; r <= 1; r++) {
		for (int c = -1; c <= 1; c++) {
			if (r == 0 && c == 0) continue;  // current position of king

			int newRow = kingRow + r;
			int newCol = kingCol + c;

			if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
				if (isBlackChecked) {
					if (board[newRow][newCol] >= 'a' && board[newRow][newCol] <= 'z') continue; // Can't move onto own piece
				}
				else {
					if (board[newRow][newCol] >= 'A' && board[newRow][newCol] <= 'Z') continue; // Can't move onto own piece
				}
				// temporarily move king to new position
				char temp = board[newRow][newCol];
				board[newRow][newCol] = board[kingRow][kingCol];
				board[kingRow][kingCol] = '.';

				int blackKingRow, blackKingCol, whiteKingRow, whiteKingCol;
				findKingPosition(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol);

				// check if king is still in check after the move
				bool blackCheck = false, whiteCheck = false;
				isKingAttacked(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck);

				// Undo move
				board[kingRow][kingCol] = board[newRow][newCol];
				board[newRow][newCol] = temp;

				if (isBlackChecked) {
					if (!blackCheck) return false; // black king can escape - not a checkmate
				}
				else {
					if (!whiteCheck) return false; // white king can escape - not a checkmate
				}
			}
		}
	}

	// check if a same coloured piece can block or capture attacking piece
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			char p = board[r][c]; // piece trying to block or capture

			if ((isBlackChecked && (p >= 'a' && p <= 'z' && p != 'k')) || (!isBlackChecked && (p >= 'A' && p <= 'Z' && p != 'K'))) {

				for (int nr = 0; nr < 8; nr++) {
					for (int nc = 0; nc < 8; nc++) {

						if (isValidMove(r, c, nr, nc, board)) {
							// temporarily move piece to new position
							char temp = board[nr][nc];
							board[nr][nc] = board[r][c];
							board[r][c] = '.';

							int blackKingRow, blackKingCol, whiteKingRow, whiteKingCol;
							findKingPosition(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol);

							// check if king is still in check after the move
							bool blackCheck = false, whiteCheck = false;
							isKingAttacked(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck);

							// Undo move
							board[r][c] = board[nr][nc];
							board[nr][nc] = temp;

							if (isBlackChecked) {
								if (!blackCheck) return false; // black king can be saved
							}
							else {
								if (!whiteCheck) return false; // white king can be saved
							}
						}
					}
				}
			}
		}
	}

	return true; // No escape - checkmate
}


void promotePawn(char board[][8], int row, int col, char& promotedPiece) {
	char piece = board[row][col];

	// when white pawn reaches row 0
	if (piece == 'P' && row == 0) {
		board[row][col] = 'Q'; // promote to Queen
		promotedPiece = 'Q';
		cout << "White Pawn promoted to Queen!" << endl;
	}
	// when black pawn reaches row 7
	else if (piece == 'p' && row == 7) {
		board[row][col] = 'q'; // promote to Queen
		promotedPiece = 'q';
		cout << "Black Pawn promoted to Queen!" << endl;
	}
}


void findPossibleMoves(char board[][8], int selectedRow, int selectedCol, bool possibleMoves[][8]) {
	// Clear previous possible moves
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			possibleMoves[i][j] = false;
		}
	}
	// Check all squares on the board
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (isValidMove(selectedRow, selectedCol, r, c, board)) {
				possibleMoves[r][c] = true;
			}
		}
	}
}


void drawPossibleMoves(RenderWindow& window, int boxSize, CircleShape moveCircle, CircleShape captureCircle,
	bool possibleMoves[][8], char board[][8], bool isWhiteTurn) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (possibleMoves[i][j]) {
				// Skip if it's a same-colored piece
				if (board[i][j] != '.') {
					if (isWhiteTurn) {
						// White's turn - skip white pieces (uppercase)
						if (board[i][j] >= 'A' && board[i][j] <= 'Z') {
							continue;
						}
					}
					else {
						// Black's turn - skip black pieces (lowercase)
						if (board[i][j] >= 'a' && board[i][j] <= 'z') {
							continue;
						}
					}
				}

				float xPos = j * boxSize + boxSize / 2.f;
				float yPos = i * boxSize + boxSize / 2.f;

				// Check if square has an opponent's piece
				bool isOpponentPiece = false;
				if (board[i][j] != '.') {
					if (isWhiteTurn) {
						// White's turn - check if target has black piece (lowercase)
						if (board[i][j] >= 'a' && board[i][j] <= 'z') {
							isOpponentPiece = true;
						}
					}
					else {
						// Black's turn - check if target has white piece (uppercase)
						if (board[i][j] >= 'A' && board[i][j] <= 'Z') {
							isOpponentPiece = true;
						}
					}
				}

				if (isOpponentPiece) {
					// Draw capture circle for opponent pieces
					captureCircle.setPosition(xPos - captureCircle.getRadius(), yPos - captureCircle.getRadius());
					window.draw(captureCircle);
				}
				else {
					// Draw move circle for empty squares
					moveCircle.setPosition(xPos - moveCircle.getRadius(), yPos - moveCircle.getRadius());
					window.draw(moveCircle);
				}
			}
		}
	}
}


bool isStalemate(char board[][8], int kingRow, int kingCol, bool isBlackKing) {
	// Check if king is in check - if yes, it's not stalemate
	int blackKingRow, blackKingCol, whiteKingRow, whiteKingCol;
	findKingPosition(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol);

	bool blackCheck = false, whiteCheck = false;
	isKingAttacked(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck);

	if (isBlackKing && blackCheck) return false;  // Black king is in check - not stalemate
	if (!isBlackKing && whiteCheck) return false; // White king is in check - not stalemate

	// Check if the player has any legal moves
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			char piece = board[r][c];

			// Check if piece belongs to current player
			if (isBlackKing) {
				if (!(piece >= 'a' && piece <= 'z')) continue;  // Not a black piece
			}
			else {
				if (!(piece >= 'A' && piece <= 'Z')) continue;  // Not a white piece
			}

			// Check all possible moves for this piece
			for (int nr = 0; nr < 8; nr++) {
				for (int nc = 0; nc < 8; nc++) {
					if (isValidMove(r, c, nr, nc, board)) {
						// Temporarily make the move
						char temp = board[nr][nc];
						board[nr][nc] = board[r][c];
						board[r][c] = '.';

						// Find king position after move
						int bkRow, bkCol, wkRow, wkCol;
						findKingPosition(board, bkRow, bkCol, wkRow, wkCol);

						// Check if king is still in check
						bool bCheck = false, wCheck = false;
						isKingAttacked(board, bkRow, bkCol, wkRow, wkCol, bCheck, wCheck);

						// Undo move
						board[r][c] = board[nr][nc];
						board[nr][nc] = temp;

						// If king is not in check after move, it's a legal move
						if (isBlackKing && !bCheck) return false;  // Black has legal move
						if (!isBlackKing && !wCheck) return false; // White has legal move
					}
				}
			}
		}
	}

	return true; // No legal moves and not in check = stalemate
}


int main() {
	int window_height = 800;
	RenderWindow window(VideoMode(window_height, window_height), "Chess Master", Style::Default);
	window.setFramerateLimit(60);


	// ICON 
	Image chessIcon;
	if (chessIcon.loadFromFile("C:/Users/DELL/source/repos/chess_project/chess_project/assets/chess_icon.png")) {
		window.setIcon(chessIcon.getSize().x, chessIcon.getSize().y, chessIcon.getPixelsPtr());
	}
	else {
		cout << "Failed to load the Icon!";
	}

	char board[8][8];
	initializeBoard(board);
	printBoard(board);


	// ---- BOXES SETUP ----
	int xPos, yPos, boxSize = window_height / 8;
	Color normalLight(238, 238, 210);
	Color normalDark(118, 150, 86);

	// Black box
	RectangleShape darkBox(Vector2f(boxSize, boxSize));
	darkBox.setFillColor(normalDark);
	// white box
	RectangleShape lightBox(Vector2f(boxSize, boxSize));
	lightBox.setFillColor(normalLight);


	// ---- FONT ----
	Font font;
	if (!font.loadFromFile("C:/Users/DELL/source/repos/chess_project/chess_project/assets/fonts/arial.ttf")) {
		cout << "Failed to load the font!" << endl;
	}


	// ---- BOX-LABELS SETUP ----
	Text label;
	label.setFont(font);
	label.setCharacterSize(boxSize / 6);
	label.setStyle(Text::Bold);


	// ---- BOX HIGHLIGHTING ARRAY SETUP ----
	bool highlight[8][8] = { false };
	Color highlightLight(245, 246, 130);
	Color highlightDark(185, 202, 67);


	// ---- Check And Checkmate colour highligh variable ----
	Color checkColor(255, 102, 102);
	Color checkmateColor(147, 112, 219);

	// ---- SETUP  FOR CIRCLING THE POSSIBLE MOVES ----
	CircleShape moveCircle(boxSize / 6);
	moveCircle.setFillColor(Color(50, 50, 50, 100));
	CircleShape captureCircle(boxSize / 2.3);
	captureCircle.setFillColor(Color::Transparent);
	captureCircle.setOutlineColor(Color(50, 50, 50, 100));
	captureCircle.setOutlineThickness(-10.f);
	bool possibleMoves[8][8] = { false };


	// ---- HOVER OUTLINE SETUP ----
	RectangleShape hoverOutline(Vector2f(boxSize, boxSize));
	hoverOutline.setFillColor(Color::Transparent);
	hoverOutline.setOutlineColor(Color(180, 180, 180, 200));
	hoverOutline.setOutlineThickness(-5.f);


	// ---- TEXTURES AND SPRITES SETUP ----
	string pieces_url[2][10] = {
		{
		"black-rook.png", "black-knight.png", "black-bishop.png",
		"black-queen.png", "black-king.png", "black-bishop.png",
		"black-knight.png", "black-rook.png", "black-pawn.png"
		},
		{
		"white-rook.png", "white-knight.png", "white-bishop.png",
		"white-queen.png", "white-king.png", "white-bishop.png",
		"white-knight.png", "white-rook.png", "white-pawn.png"
		}
	};
	string base_url = "C:/Users/DELL/source/repos/chess_project/chess_project/assets/new_pieces/";


	// row 0 is for black and row 1 is for white
	Texture pieces_textures[2][16];
	Sprite pieces[2][16];

	// ---- LOADING PIECES INTO ARRAYS OF TEXTUES AND SPRITES ----

	// black and white pieces (1st and 8th row)
	for (int i = 0; i < 8; i++) {
		// black
		if (!pieces_textures[0][i].loadFromFile(base_url + pieces_url[0][i])) {
			cout << "Failed to load " << pieces_url[0][i] << endl;
		}
		else {
			pieces_textures[0][i].setSmooth(true);
			pieces[0][i].setTexture(pieces_textures[0][i]);
			pieces[0][i].setScale(0.33, 0.33);
		}

		// white
		if (!pieces_textures[1][i].loadFromFile(base_url + pieces_url[1][i])) {
			cout << "Failed to load " << pieces_url[1][i] << endl;
		}
		else {
			pieces_textures[1][i].setSmooth(true);
			pieces[1][i].setTexture(pieces_textures[1][i]);
			pieces[1][i].setScale(0.33, 0.33);
		}
	}
	// black and white pawns (2nd and 7th row)
	for (int i = 8; i < 16; i++) {
		// black
		if (!pieces_textures[0][i].loadFromFile(base_url + pieces_url[0][8])) {
			cout << "Failed to load " << pieces_url[0][8] << endl;
		}
		else {
			pieces_textures[0][i].setSmooth(true);
			pieces[0][i].setTexture(pieces_textures[0][i]);
			pieces[0][i].setScale(0.33, 0.33);
		}

		// white
		if (!pieces_textures[1][i].loadFromFile(base_url + pieces_url[1][8])) {
			cout << "Failed to load " << pieces_url[1][8] << endl;
		}
		else {
			pieces_textures[1][i].setSmooth(true);
			pieces[1][i].setTexture(pieces_textures[1][i]);
			pieces[1][i].setScale(0.33, 0.33);
		}
	}


	//	---- SETTING INITIAL POSITIONS OF PIECES ----
	for (int i = 0; i < 8; i++) {
		// black pieces (1st row)
		if (i == 0) {
			for (int j = 0; j < 8; j++) {
				FloatRect bounds = pieces[0][j].getLocalBounds();
				pieces[0][j].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
				pieces[0][j].setPosition(j * boxSize + boxSize / 2.f, i * boxSize + boxSize / 2.f);
			}
		}
		// black pawns (2nd row)
		if (i == 1) {
			for (int j = 8; j < 16; j++) {
				FloatRect bounds = pieces[0][j].getLocalBounds();
				pieces[0][j].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
				pieces[0][j].setPosition((j - 8) * boxSize + boxSize / 2.f, i * boxSize + boxSize / 2.f);
			}
		}
		// white pawns (7th row)
		if (i == 6) {
			for (int j = 8; j < 16; j++) {
				FloatRect bounds = pieces[1][j].getLocalBounds();
				pieces[1][j].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
				pieces[1][j].setPosition((j - 8) * boxSize + boxSize / 2.f, i * boxSize + boxSize / 2.f);
			}
		}
		// white pieces (8th row)
		if (i == 7) {
			for (int j = 0; j < 8; j++) {
				FloatRect bounds = pieces[1][j].getLocalBounds();
				pieces[1][j].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
				pieces[1][j].setPosition(j * boxSize + boxSize / 2.f, i * boxSize + boxSize / 2.f);
			}
		}
	}

	bool isDragging = false;
	int selectedPiece = -1;
	bool isWhite;
	Vector2f originalPos;


	View view(FloatRect(0, 0, window_height, window_height));

	int oldRow, oldCol, newRow, newCol;
	char temp;

	bool isWhiteTurn = true;

	bool gameOver = false;


	// ---- GAME LOOP ----
	while (window.isOpen()) {
		Event event;

		// ---- EVENT LOOP ----
		while (window.pollEvent(event)) {

			// close window
			if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
				window.close();
			}

			// When window is resized
			if (event.type == Event::Resized) {
				float width = static_cast<float>(event.size.width);
				float height = static_cast<float>(event.size.height);
				float size = min(width, height);
				float offsetX = (width - size) / 2.f;
				float offsetY = (height - size) / 2.f;

				view.setViewport(FloatRect(offsetX / width, offsetY / height, size / width, size / height));
				window.setView(view);
			}

			// drag pieces
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !gameOver) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

				for (int i = 0; i < 16; i++) {
					if (pieces[0][i].getGlobalBounds().contains(mousePos)) {
						selectedPiece = i;
						isDragging = true;
						isWhite = false;
						originalPos = pieces[0][i].getPosition();
						break;
					}
					else if (pieces[1][i].getGlobalBounds().contains(mousePos)) {
						selectedPiece = i;
						isDragging = true;
						originalPos = pieces[1][i].getPosition();
						isWhite = true;
						break;
					}
				}

				oldRow = originalPos.y / boxSize;
				oldCol = originalPos.x / boxSize;

				// change box color on click

				int row = mousePos.y / boxSize;
				int col = mousePos.x / boxSize;


				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						highlight[i][j] = false;
						possibleMoves[i][j] = false;
					}
				}

				if (row >= 0 && row < 8 && col >= 0 && col < 8) {
					bool pieceFound = false;

					// Check black pieces
					for (int i = 0; i < 16; i++) {
						int pieceRow = pieces[0][i].getPosition().y / boxSize;
						int pieceCol = pieces[0][i].getPosition().x / boxSize;

						if (pieceRow == row && pieceCol == col) {
							// Only show highlights if it's black's turn
							if (!isWhiteTurn) {
								highlight[row][col] = true;
								findPossibleMoves(board, row, col, possibleMoves);
							}
							pieceFound = true;
							break;
						}
					}

					// Check white pieces
					if (!pieceFound) {
						for (int i = 0; i < 16; i++) {
							int pieceRow = pieces[1][i].getPosition().y / boxSize;
							int pieceCol = pieces[1][i].getPosition().x / boxSize;

							if (pieceRow == row && pieceCol == col) {
								// Only show highlights if it's white's turn
								if (isWhiteTurn) {
									highlight[row][col] = true;
									findPossibleMoves(board, row, col, possibleMoves);
								}
								break;
							}
						}
					}
				}
			}

			if (event.type == Event::MouseButtonReleased && selectedPiece != -1 && !gameOver) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

				// Calculate which square the mouse was released over
				int col = mousePos.x / boxSize;
				int row = mousePos.y / boxSize;

				newRow = row;
				newCol = col;

				float newX = col * boxSize + boxSize / 2.f;
				float newY = row * boxSize + boxSize / 2.f;

				bool sameColorCollision = false;

				// check if target square has same-colored piece
				if (isWhite) {
					for (int i = 0; i < 16; i++) {
						if (i == selectedPiece) continue;
						if (pieces[1][i].getGlobalBounds().contains(newX, newY)) {
							sameColorCollision = true;
							break;
						}
					}
				}
				else if (!isWhite) {
					for (int i = 0; i < 16; i++) {
						if (i == selectedPiece) continue;
						if (pieces[0][i].getGlobalBounds().contains(newX, newY)) {
							sameColorCollision = true;
							break;
						}
					}
				}

				bool isMoveValid = isValidMove(oldRow, oldCol, newRow, newCol, board);

				// if same color collision then place the piece back to its original position
				if (sameColorCollision || (isWhite != isWhiteTurn) || !isMoveValid) {
					if (isWhite)
						pieces[1][selectedPiece].setPosition(originalPos);
					else
						pieces[0][selectedPiece].setPosition(originalPos);
				}
				else {
					// otherwise move to new position
					if (isWhite)
						pieces[1][selectedPiece].setPosition(newX, newY);
					else
						pieces[0][selectedPiece].setPosition(newX, newY);

					if (isMoveValid && (newRow != oldRow || newCol != oldCol)) {
						isWhiteTurn = !isWhiteTurn;
					}

					if (isWhite) {
						for (int i = 0; i < 16; i++) {
							if (pieces[1][selectedPiece].getPosition() == pieces[0][i].getPosition()) {
								pieces[0][i].setPosition(-1000, -1000);
								break;
							}
						}
					}
					else {
						for (int i = 0; i < 16; i++) {
							if (pieces[0][selectedPiece].getPosition() == pieces[1][i].getPosition()) {
								pieces[1][i].setPosition(-1000, -1000);
								break;
							}
						}
					}

					// update pieces in board array and print on console
					temp = board[oldRow][oldCol];
					board[oldRow][oldCol] = '.';
					board[newRow][newCol] = temp;


					char promotedPiece = ' ';
					promotePawn(board, newRow, newCol, promotedPiece);

					// If pawn was promoted, update the piece sprite
					if (promotedPiece == 'Q' || promotedPiece == 'q') {
						// Load the queen texture for the promoted pawn
						Texture queenTexture;
						string queenPath;

						if (isWhite)
							queenPath = base_url + "white-queen.png";
						else
							queenPath = base_url + "black-queen.png";

						if (queenTexture.loadFromFile(queenPath)) {
							queenTexture.setSmooth(true);

							if (isWhite) {
								pieces_textures[1][selectedPiece] = queenTexture;
								pieces[1][selectedPiece].setTexture(pieces_textures[1][selectedPiece]);
								pieces[1][selectedPiece].setScale(0.33, 0.33);
								FloatRect bounds = pieces[1][selectedPiece].getLocalBounds();
								pieces[1][selectedPiece].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
							}
							else {
								pieces_textures[0][selectedPiece] = queenTexture;
								pieces[0][selectedPiece].setTexture(pieces_textures[0][selectedPiece]);
								pieces[0][selectedPiece].setScale(0.33, 0.33);
								FloatRect bounds = pieces[0][selectedPiece].getLocalBounds();
								pieces[0][selectedPiece].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
							}
						}
					}

					printBoard(board);

					// find king position
					int blackKingRow, blackKingCol, whiteKingRow, whiteKingCol;
					findKingPosition(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol);

					// check for check
					bool blackCheck = false; // when black king is under attack
					bool whiteCheck = false; // when white king is under attack
					isKingAttacked(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck);

					if (blackCheck)
						cout << "Black is in Check!" << endl;
					if (whiteCheck)
						cout << "White is in Check!" << endl;

					// check for checkmate
					if (blackCheck) {
						if (isCheckmate(board, blackKingRow, blackKingCol)) {
							cout << "Checkmate! White wins!" << endl;
							gameOver = true;
						}
					}
					else if (whiteCheck) {
						if (isCheckmate(board, whiteKingRow, whiteKingCol)) {
							cout << "Checkmate! Black wins!" << endl;
							gameOver = true;
						}
					}

					// check for stalemate
					if (!blackCheck && !whiteCheck) {
						if (isWhiteTurn && isStalemate(board, whiteKingRow, whiteKingCol, false)) {
							cout << "Stalemate! Game is a Draw!" << endl;
							gameOver = true;
						}
						else if (!isWhiteTurn && isStalemate(board, blackKingRow, blackKingCol, true)) {
							cout << "Stalemate! Game is a Draw!" << endl;
							gameOver = true;
						}
					}

				}


				selectedPiece = -1;
				isDragging = false;

				highlight[row][col] = true;

				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						possibleMoves[i][j] = false;
					}
				}
			}


		}


		if (isDragging) {
			Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

			int row = mousePos.y / boxSize;
			int col = mousePos.x / boxSize;

			if (isWhite)
				pieces[1][selectedPiece].setPosition(mousePos);
			else
				pieces[0][selectedPiece].setPosition(mousePos);

			if (col >= 0 && col < 8 && row >= 0 && row < 8) {
				// Set the outline position to match the top-left of the hovered square
				hoverOutline.setPosition(col * boxSize, row * boxSize);
			}
		}


		window.clear(Color(63, 79, 47));

		// ---- DRAW EVERYTHING ----

		int blackKingRow, blackKingCol, whiteKingRow, whiteKingCol;
		findKingPosition(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol);

		bool blackCheck = false, whiteCheck = false;
		isKingAttacked(board, blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck);

		// drawing board
		drawBoard(window, boxSize, lightBox, darkBox,
			normalLight, normalDark, highlight, highlightLight, highlightDark, label,
			blackKingRow, blackKingCol, whiteKingRow, whiteKingCol, blackCheck, whiteCheck, gameOver, checkColor, checkmateColor);

		// draw hover outline
		if (isDragging) {
			window.draw(hoverOutline);
		}

		// drawing pieces
		for (int i = 0; i < 16; i++) {
			window.draw(pieces[0][i]);
			window.draw(pieces[1][i]);
		}

		// Draw the dragged piece last so it appears on top
		if (isDragging && selectedPiece != -1) {
			if (isWhite) {
				window.draw(pieces[1][selectedPiece]);
			}
			else {
				window.draw(pieces[0][selectedPiece]);
			}
		}

		drawPossibleMoves(window, boxSize, moveCircle, captureCircle, possibleMoves, board, isWhiteTurn);

		window.display();

	}

	return 0;
}