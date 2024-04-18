#include "GUI.hpp"


GUI::GUI(Game game) : m_window({ 800u, 800u }, "Chess"), m_game(game){
    m_window.setFramerateLimit(60);
    generate_textures();
    while (m_window.isOpen())
    {
        for (auto event = sf::Event{}; m_window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }
        }
        m_window.clear();
        // draw sth
        draw_chessboard(100.0);
        draw_pieces(100.0);
        m_window.display();
    }
}

void GUI::generate_textures() {
    //white pieces
    m_textures['K'] = sf::Texture();
    if (!m_textures['K'].loadFromFile("../../textures/white_king.png"))
        return;
    m_textures['Q'] = sf::Texture();
    if (!m_textures['Q'].loadFromFile("../../textures/white_queen.png"))
        return;
    m_textures['B'] = sf::Texture();
    if (!m_textures['B'].loadFromFile("../../textures/white_bishop.png"))
        return;
    m_textures['N'] = sf::Texture();
    if (!m_textures['N'].loadFromFile("../../textures/white_kinght.png"))
        return;
    m_textures['R'] = sf::Texture();
    if (!m_textures['R'].loadFromFile("../../textures/white_rook.png"))
        return;
    m_textures['P'] = sf::Texture();
    if (!m_textures['P'].loadFromFile("../../textures/white_pawn.png"))
        return;

    //black pieces
    m_textures['k'] = sf::Texture();
    if (!m_textures['k'].loadFromFile("../../textures/black_king.png"))
        return;
    m_textures['q'] = sf::Texture();
    if (!m_textures['q'].loadFromFile("../../textures/black_queen.png"))
        return;
    m_textures['b'] = sf::Texture();
    if (!m_textures['b'].loadFromFile("../../textures/black_bishop.png"))
        return;
    m_textures['n'] = sf::Texture();
    if (!m_textures['n'].loadFromFile("../../textures/black_kinght.png"))
        return;
    m_textures['r'] = sf::Texture();
    if (!m_textures['r'].loadFromFile("../../textures/black_rook.png"))
        return;
    m_textures['p'] = sf::Texture();
    if (!m_textures['p'].loadFromFile("../../textures/black_pawn.png"))
        return;
}


void GUI::draw_chessboard(float squareSize) {
    m_window.RenderTarget::clear(sf::Color(169,169,169));
    for(std::size_t y = 0; y<=7;y++) {
        for (std::size_t x = 0; x <=3; x++) {
            sf::RectangleShape square;
            square.setSize(sf::Vector2f(squareSize, squareSize));
            square.setFillColor(sf::Color(249, 246, 238));
            square.setPosition(((x * 2 * squareSize) + squareSize * (y % 2)),
                               (y * squareSize));
            m_window.draw(square);
        }
    }
}

void GUI::draw_pieces(float squareSize) {
    Piece::PiecesMap piecesMap = m_game.get_pieces_map();

    for(auto elem : piecesMap){
        int xPos = (elem.first.first-1) * 100;
        int yPos = (elem.first.second-1) * 100;
        sf::RectangleShape piece;
        piece.setSize(sf::Vector2f(squareSize, squareSize));
        piece.setTexture(&m_textures[elem.second->get_id()]);
        piece.setPosition(xPos,
                          yPos);
        m_window.draw(piece);
    }


}




