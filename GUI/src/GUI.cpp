#include "GUI/GUI.hpp"


GUI::GUI(GameInterface gameInterface, int squareSize = 100)
        : m_window({800u, 800u}, "Chess"),
          m_gameInterface(gameInterface), m_squareSize(squareSize), m_clickedSquare(0, 0) {
    m_window.setFramerateLimit(60);
    generate_textures();
    while (m_window.isOpen()) {
        for (auto event = sf::Event{}; m_window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                left_mouse_button_clicked();
            }
        }
        m_window.clear();
        // draw sth
        draw_chessboard();
        draw_possible_moves();
        draw_pieces();
        m_window.display();
    }
}

void GUI::generate_textures() {
    //white pieces
    m_textures['K'] = sf::Texture();
    if (!m_textures['K'].loadFromFile("textures/white_king.png"))
        return;
    m_textures['Q'] = sf::Texture();
    if (!m_textures['Q'].loadFromFile("textures/white_queen.png"))
        return;
    m_textures['B'] = sf::Texture();
    if (!m_textures['B'].loadFromFile("textures/white_bishop.png"))
        return;
    m_textures['N'] = sf::Texture();
    if (!m_textures['N'].loadFromFile("textures/white_kinght.png"))
        return;
    m_textures['R'] = sf::Texture();
    if (!m_textures['R'].loadFromFile("textures/white_rook.png"))
        return;
    m_textures['P'] = sf::Texture();
    if (!m_textures['P'].loadFromFile("textures/white_pawn.png"))
        return;

    //black pieces
    m_textures['k'] = sf::Texture();
    if (!m_textures['k'].loadFromFile("textures/black_king.png"))
        return;
    m_textures['q'] = sf::Texture();
    if (!m_textures['q'].loadFromFile("textures/black_queen.png"))
        return;
    m_textures['b'] = sf::Texture();
    if (!m_textures['b'].loadFromFile("textures/black_bishop.png"))
        return;
    m_textures['n'] = sf::Texture();
    if (!m_textures['n'].loadFromFile("textures/black_kinght.png"))
        return;
    m_textures['r'] = sf::Texture();
    if (!m_textures['r'].loadFromFile("textures/black_rook.png"))
        return;
    m_textures['p'] = sf::Texture();
    if (!m_textures['p'].loadFromFile("textures/black_pawn.png"))
        return;
}


void GUI::draw_chessboard() {
    m_window.RenderTarget::clear(sf::Color(169, 169, 169));
    for (std::size_t y = 0; y <= 7; y++) {
        for (std::size_t x = 0; x <= 3; x++) {
            sf::RectangleShape square;
            square.setSize(sf::Vector2f(m_squareSize, m_squareSize));
            square.setFillColor(sf::Color(249, 246, 238));
            square.setPosition(((x * 2 * m_squareSize) + m_squareSize * (y % 2)),
                               (y * m_squareSize));
            m_window.draw(square);
        }
    }
}

void GUI::draw_pieces() {
    std::map<Position, char> allPieces = m_gameInterface.getPiecesAsChar();

    for (auto elem: allPieces) {
        int xPos = (elem.first.x - 1) * m_squareSize;
        int yPos = (elem.first.y - 1) * m_squareSize;
        sf::RectangleShape piece;
        piece.setSize(sf::Vector2f(m_squareSize, m_squareSize));
        piece.setTexture(&m_textures[elem.second]);
        piece.setPosition(xPos,
                          yPos);
        m_window.draw(piece);
    }


}

Position GUI::get_mouse_position() {
    sf::Vector2i position = sf::Mouse::getPosition(m_window);
    Position pos(((int) position.x / 100) + 1, ((int) position.y / 100) + 1);
    return pos;
}

void GUI::draw_possible_moves() {

    std::vector<Move> possibleMoves = m_gameInterface.getPiecePossibleMoves(m_clickedSquare);
    for (auto move: possibleMoves) {
        sf::RectangleShape square;
        square.setSize(sf::Vector2f(m_squareSize, m_squareSize));
        square.setFillColor(sf::Color(201, 77, 68, 128));
        square.setPosition((move.endPosition.x - 1) * m_squareSize,
                           (move.endPosition.y - 1) * m_squareSize);
        m_window.draw(square);
    }

}

void GUI::left_mouse_button_clicked() {
    Position newClickedSquare = get_mouse_position();
    // m_gameInterface.move(m_clickedSquare, newClickedSquare);

    m_clickedSquare = newClickedSquare;
    // std::cout<<m_clickedSquare.x<<","<<m_clickedSquare.y<<"\n";
}













