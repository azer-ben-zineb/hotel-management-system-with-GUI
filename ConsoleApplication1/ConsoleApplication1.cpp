#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <functional>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <regex>
#include <map>
#include <memory>
namespace Config {
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 700;
    constexpr float GRID_SIZE = 8.0f;
    const std::string WINDOW_TITLE = "Neon Hotel Management | Cyberpunk v1.0";


    const sf::Color BG_DARK(10, 10, 15);
    const sf::Color PANEL_BG(18, 18, 26, 240);
    const sf::Color NEON_CYAN(0, 240, 255);
    const sf::Color NEON_MAGENTA(255, 0, 170);
    const sf::Color NEON_PURPLE(176, 0, 255);
    const sf::Color NEON_GREEN(0, 255, 136);
    const sf::Color NEON_YELLOW(255, 230, 0);
    const sf::Color NEON_RED(255, 32, 80);
    const sf::Color TEXT_PRIMARY(224, 224, 255);
    const sf::Color TEXT_SECONDARY(106, 106, 138);
    const sf::Color BORDER_COLOR(42, 42, 58);
    const sf::Color INPUT_BG(13, 13, 20);
}


class RoundedRectangleShape : public sf::Shape {
public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = {0, 0}, float radius = 0, unsigned int cornerPointCount = 8)
        : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) { m_size = size; update(); }
    const sf::Vector2f& getSize() const { return m_size; }
    void setCornerRadius(float radius) { m_radius = radius; update(); }
    
    virtual std::size_t getPointCount() const override { return m_cornerPointCount * 4; }
    
    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= m_cornerPointCount * 4) return {0, 0};
        
        float deltaAngle = 90.0f / (m_cornerPointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = static_cast<unsigned int>(index / m_cornerPointCount);
        unsigned int pointIndex = index % m_cornerPointCount;
        

        switch(centerIndex) {
            case 0: center = {m_size.x - m_radius, m_radius}; break;
            case 1: center = {m_size.x - m_radius, m_size.y - m_radius}; break;
            case 2: center = {m_radius, m_size.y - m_radius}; break;
            case 3: center = {m_radius, m_radius}; break;
        }


        float angle = 0.f;
        switch(centerIndex) {
            case 0: angle = 270.0f + pointIndex * deltaAngle; break;
            case 1: angle = 0.0f + pointIndex * deltaAngle; break;
            case 2: angle = 90.0f + pointIndex * deltaAngle; break;
            case 3: angle = 180.0f + pointIndex * deltaAngle; break;
        }

        float rad = angle * 3.14159265f / 180.0f;
        return {center.x + m_radius * std::cos(rad), center.y + m_radius * std::sin(rad)};
    }

private:
    sf::Vector2f m_size;
    float m_radius;
    unsigned int m_cornerPointCount;
};



class Guest {
public:
    int id;
    std::string name, phone, email;
    Guest(int i, std::string n, std::string p, std::string e) : id(i), name(n), phone(p), email(e) {}
};

class Room {
public:
    int number;
    std::string type;
    double price;
    bool available;
    
    Room(int n, std::string t, double p) : number(n), type(t), price(p), available(true) {}
    virtual ~Room() = default;
    virtual std::string getAmenities() const { return "Basic"; }
};

class StandardRoom : public Room {
public:
    StandardRoom(int n) : Room(n, "Standard", 100.0) {}
    std::string getAmenities() const override { return "WiFi"; }
};

class DeluxeRoom : public Room {
public:
    DeluxeRoom(int n) : Room(n, "Deluxe", 200.0) {}
    std::string getAmenities() const override { return "WiFi, Mini-bar, City View"; }
};

class SuiteRoom : public Room {
public:
    SuiteRoom(int n) : Room(n, "Suite", 350.0) {}
    std::string getAmenities() const override { return "WiFi, Mini-bar, Ocean View, Living Room"; }
};

class Reservation {
public:
    int id, guestId, roomNumber, nights;
    std::string checkIn, checkOut, mealPref;
    double totalAmount;

    Reservation(int mid, int gid, int rn, std::string ci, std::string co, int n, double amt, std::string mp)
        : id(mid), guestId(gid), roomNumber(rn), checkIn(ci), checkOut(co), nights(n), totalAmount(amt), mealPref(mp) {}
};

class HotelSystem {
public:
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<Guest> guests;
    std::vector<Reservation> reservations;
    int nextGuestId = 1001;
    int nextResId = 2001;

    HotelSystem() {
        for(int i=101; i<=110; ++i) rooms.push_back(std::make_unique<StandardRoom>(i));
        for(int i=201; i<=210; ++i) rooms.push_back(std::make_unique<DeluxeRoom>(i));
        for(int i=301; i<=305; ++i) rooms.push_back(std::make_unique<SuiteRoom>(i));
    }


    std::pair<bool, std::string> addGuest(const std::string& n, const std::string& p, const std::string& e) {
        if (!isDigitsOnly(p)) return {false, "Invalid Phone: Digits only"};
        if (!isValidEmail(e)) return {false, "Invalid Email format"};
        guests.emplace_back(nextGuestId++, n, p, e);
        return {true, "Guest Added! ID: " + std::to_string(nextGuestId - 1)};
    }
    
    std::pair<bool, std::string> makeReservation(int gId, int rNum, const std::string& in, const std::string& out, int nights, bool b, bool l, bool d) {
        auto gIt = std::find_if(guests.begin(), guests.end(), [&](const Guest& g){ return g.id == gId; });
        if(gIt == guests.end()) return {false, "Guest ID not found"};
        
        auto rIt = std::find_if(rooms.begin(), rooms.end(), [&](const std::unique_ptr<Room>& r){ return r->number == rNum; });
        if(rIt == rooms.end()) return {false, "Room not found"};
        if(!(*rIt)->available) return {false, "Room occupied"};

        if(!isValidDate(in) || !isValidDate(out)) return {false, "Invalid Date (DD/MM/YYYY)"};

        std::string meals;
        if(b) meals += "Bfst ";
        if(l) meals += "Lnch ";
        if(d) meals += "Dinn";
        if (nights == 1 && b) meals = "Bfst (Only)";

        double total = (*rIt)->price * nights;
        reservations.emplace_back(nextResId++, gId, rNum, in, out, nights, total, meals);
        (*rIt)->available = false;
        
        std::stringstream ss;
        ss << "Reserved! ID: " << (nextResId-1) << " Cost: $" << std::fixed << std::setprecision(2) << total;
        return {true, ss.str()};
    }

    std::pair<bool, std::string> checkOut(int rKey) {
        auto it = std::find_if(reservations.begin(), reservations.end(), [&](const Reservation& r){ return r.id == rKey; });
        if(it == reservations.end()) return {false, "Reservation not found"};
        
        int rNum = it->roomNumber;
        auto roomIt = std::find_if(rooms.begin(), rooms.end(), [&](const std::unique_ptr<Room>& r){ return r->number == rNum; });
        if(roomIt != rooms.end()) (*roomIt)->available = true;
        
        reservations.erase(it);
        return {true, "Checked Out Successfully"};
    }
    
    std::pair<bool, std::string> deleteGuest(int gId) {
        auto it = std::find_if(guests.begin(), guests.end(), [&](const Guest& g){ return g.id == gId; });
        if(it == guests.end()) return {false, "Guest not found"};
        guests.erase(it);
        return {true, "Guest Deleted"};
    }
    
private:
    bool isDigitsOnly(const std::string& s) { return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit); }
    bool isValidEmail(const std::string& e) { return e.find('@') != std::string::npos && e.find('.') > e.find('@')+1; }
    
    bool isLeap(int y) { return (y%4==0 && (y%100!=0 || y%400==0)); }
    bool isValidDate(const std::string& d) {
        int day, m, y; char s1, s2;
        std::stringstream ss(d);
        if(!(ss >> day >> s1 >> m >> s2 >> y) || s1!='/' || s2!='/') return false;
        if(y < 2026 || m<1 || m>12) return false;
        int dim[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if(isLeap(y)) dim[2]=29;
        return day>=1 && day<=dim[m];
    }
};



class NeonButton {
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered = false;
    std::function<void()> onClick;

    NeonButton(const sf::Font& font, const std::string& label, const sf::Vector2f& pos, const sf::Vector2f& size) 
        : text(font) {
        shape.setPosition(pos);
        shape.setSize(size);
        shape.setFillColor(Config::PANEL_BG);
        shape.setOutlineThickness(1.5f);
        shape.setOutlineColor(Config::BORDER_COLOR);
        
        text.setString(label);
        text.setCharacterSize(14);
        text.setFillColor(Config::TEXT_PRIMARY);
        

        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition({
            pos.x + (size.x - bounds.size.x) / 2.0f - bounds.position.x,
            pos.y + (size.y - bounds.size.y) / 2.0f - bounds.position.y
        });
    }

    void update(const sf::Vector2f& mousePos, bool clicked) {
        isHovered = shape.getGlobalBounds().contains(mousePos);
        if (isHovered) {
            shape.setOutlineColor(Config::NEON_CYAN);
            shape.setOutlineThickness(2.0f);
            if(clicked && onClick) onClick();
        } else {
            shape.setOutlineColor(Config::BORDER_COLOR);
            shape.setOutlineThickness(1.5f);
        }
    }

    void draw(sf::RenderWindow& window) {
        // Glow effect if hovered
        if (isHovered) {
            sf::RectangleShape glow = shape;
            glow.setFillColor(sf::Color::Transparent);
            glow.setOutlineColor(sf::Color(Config::NEON_CYAN.r, Config::NEON_CYAN.g, Config::NEON_CYAN.b, 100));
            glow.setOutlineThickness(4.f);
            window.draw(glow);
        }
        window.draw(shape);
        window.draw(text);
    }
};

class NeonTextInput {
public:
    RoundedRectangleShape shape;
    sf::Text displayText;
    sf::Text labelText;
    std::string value;
    bool isFocused = false;
    sf::Clock blinkClock;
    bool showCursor = true;
    
    NeonTextInput(const sf::Font& font, const std::string& label, const sf::Vector2f& pos, float width)
        : displayText(font), labelText(font) {
        shape.setPosition(pos);
        shape.setSize({width, 32.f});
        shape.setCornerRadius(4.f);
        shape.setFillColor(Config::INPUT_BG);
        shape.setOutlineThickness(1.0f);
        shape.setOutlineColor(Config::BORDER_COLOR);
        
        labelText.setString(label);
        labelText.setCharacterSize(12);
        labelText.setFillColor(Config::NEON_CYAN);
        labelText.setPosition({pos.x, pos.y - 18.f});
        
        displayText.setCharacterSize(14);
        displayText.setFillColor(Config::TEXT_PRIMARY);
        displayText.setPosition({pos.x + 8.f, pos.y + 6.f});
    }

    void handleEvent(const sf::Event& event) {
        if (!isFocused) return;
        
        if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
            if (textEvent->unicode < 128) {
                char typed = static_cast<char>(textEvent->unicode);
                if (typed == 8) {
                    if (!value.empty()) value.pop_back();
                } else if (typed == 13 || typed == 9) {
                    isFocused = false;
                } else if (std::isprint(typed)) {
                    value += typed;
                }
            }
        }
    }
    
    void update(const sf::Vector2f& mousePos, bool clicked) {
        if (clicked) {
            bool wasFocused = isFocused;
            isFocused = shape.getGlobalBounds().contains(mousePos);
            if (!wasFocused && isFocused) {

            }
        }
        
        shape.setOutlineColor(isFocused ? Config::NEON_CYAN : Config::BORDER_COLOR);
        
        if (isFocused && blinkClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            blinkClock.restart();
        }
        
        displayText.setString(value + (isFocused && showCursor ? "|" : ""));
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(labelText);
        window.draw(displayText);
    }
};

class NeonCheckbox {
public:
    sf::RectangleShape box;
    sf::Text label;
    bool checked = false;
    
    NeonCheckbox(const sf::Font& font, const std::string& text, sf::Vector2f pos)
        : label(font) {
        box.setPosition(pos);
        box.setSize({16.f, 16.f});
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(Config::NEON_MAGENTA);
        box.setOutlineThickness(1.f);
        
        label.setString(text);
        label.setCharacterSize(13);
        label.setFillColor(Config::TEXT_PRIMARY);
        label.setPosition({pos.x + 24.f, pos.y - 1.f});
    }
    
    void update(const sf::Vector2f& mPos, bool clicked) {
        if (clicked && box.getGlobalBounds().contains(mPos)) {
            checked = !checked;
        }
        box.setFillColor(checked ? Config::NEON_MAGENTA : sf::Color::Transparent);
    }
    
    void draw(sf::RenderWindow& w) {
        w.draw(box);
        w.draw(label);
    }
};


enum class AppState { HOME, ADD_GUEST, VIEW_ROOMS, RESERVATION, GUESTS_LIST, RES_LIST, STATS };


int main() {
    sf::RenderWindow window(sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}), Config::WINDOW_TITLE);
    window.setFramerateLimit(60);

    
    sf::Font font;
    bool fontLoaded = false;

    if (font.openFromFile("arial.ttf") || 
        font.openFromFile("C:/Windows/Fonts/arial.ttf") || 
        font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {
        fontLoaded = true;
    }
    
    if (!fontLoaded) {
        std::cerr << "CRITICAL: No font found. Please place 'arial.ttf' in the directory." << std::endl;
 
    }

    HotelSystem hotel;
    AppState currentState = AppState::HOME;
    std::string statusMessage = "System Ready...";
    sf::Clock statusClock;
    

    sf::Text statusText(font);
    statusText.setCharacterSize(13);
    statusText.setPosition({20.f, Config::WINDOW_HEIGHT - 30.f});
    statusText.setFillColor(Config::NEON_GREEN);

    auto setStatus = [&](const std::string& msg, bool isError = false) {
        statusMessage = msg;
        statusText.setString(msg);
        statusText.setFillColor(isError ? Config::NEON_RED : Config::NEON_GREEN);
        statusClock.restart();
    };


    std::vector<NeonButton> navButtons;
    const std::vector<std::pair<std::string, AppState>> menuItems = {
        {"Home", AppState::HOME},
        {"Add Guest", AppState::ADD_GUEST},
        {"Rooms", AppState::VIEW_ROOMS},
        {"Reserve", AppState::RESERVATION},
        {"Guests", AppState::GUESTS_LIST},
        {"Bookings", AppState::RES_LIST},
        {"Stats", AppState::STATS}
    };
    
    float navY = 60.f;
    for (const auto& item : menuItems) {
        NeonButton btn(font, item.first, {20.f, navY}, {140.f, 35.f});
        btn.onClick = [&, item](){ 
            currentState = item.second; 
            setStatus("Navigated to " + item.first);
        };
        navButtons.push_back(btn);
        navY += 45.f;
    }


    NeonTextInput inpName(font, "Name", {300.f, 100.f}, 300.f);
    NeonTextInput inpPhone(font, "Phone", {300.f, 170.f}, 300.f);
    NeonTextInput inpEmail(font, "Email", {300.f, 240.f}, 300.f);
    NeonButton btnAddGuest(font, "Submit Guest", {300.f, 300.f}, {150.f, 35.f});
    btnAddGuest.onClick = [&]() {
        auto res = hotel.addGuest(inpName.value, inpPhone.value, inpEmail.value);
        setStatus(res.second, !res.first);
        if (res.first) { inpName.value = ""; inpPhone.value = ""; inpEmail.value = ""; }
    };
    

    NeonTextInput resGuestId(font, "Guest ID", {300.f, 100.f}, 100.f);
    NeonTextInput resRoomNum(font, "Room #", {420.f, 100.f}, 100.f);
    NeonTextInput resCheckIn(font, "Check In (DD/MM/YYYY)", {300.f, 170.f}, 180.f);
    NeonTextInput resCheckOut(font, "Check Out", {500.f, 170.f}, 180.f);
    NeonTextInput resNights(font, "Nights", {300.f, 240.f}, 80.f);
    
    NeonCheckbox chkBreakfast(font, "Breakfast", {300.f, 300.f});
    NeonCheckbox chkLunch(font, "Lunch", {400.f, 300.f});
    NeonCheckbox chkDinner(font, "Dinner", {500.f, 300.f});

    NeonButton btnReserve(font, "Make Reservation", {300.f, 340.f}, {200.f, 35.f});
    btnReserve.onClick = [&]() {
        try {
            if(resGuestId.value.empty() || resRoomNum.value.empty()) throw std::runtime_error("Empty fields");
            int gId = std::stoi(resGuestId.value);
            int rNum = std::stoi(resRoomNum.value);
            int n = std::stoi(resNights.value);
            auto res = hotel.makeReservation(gId, rNum, resCheckIn.value, resCheckOut.value, n, chkBreakfast.checked, chkLunch.checked, chkDinner.checked);
            setStatus(res.second, !res.first);
        } catch(...) { setStatus("Invalid numeric input", true); }
    };


    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            

            inpName.handleEvent(*event);
            inpPhone.handleEvent(*event);
            inpEmail.handleEvent(*event);
            
            resGuestId.handleEvent(*event);
            resRoomNum.handleEvent(*event);
            resCheckIn.handleEvent(*event);
            resCheckOut.handleEvent(*event);
            resNights.handleEvent(*event);
        }
        

        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        bool mouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        

        static bool wasClicked = false;
        bool justClicked = mouseClicked && !wasClicked;
        wasClicked = mouseClicked;

        for(auto& btn : navButtons) btn.update(mousePos, justClicked);
        
        if (currentState == AppState::ADD_GUEST) {
            inpName.update(mousePos, justClicked);
            inpPhone.update(mousePos, justClicked);
            inpEmail.update(mousePos, justClicked);
            btnAddGuest.update(mousePos, justClicked);
        }
        else if (currentState == AppState::RESERVATION) {
            resGuestId.update(mousePos, justClicked);
            resRoomNum.update(mousePos, justClicked);
            resCheckIn.update(mousePos, justClicked);
            resCheckOut.update(mousePos, justClicked);
            resNights.update(mousePos, justClicked);
            chkBreakfast.update(mousePos, justClicked);
            chkLunch.update(mousePos, justClicked);
            chkDinner.update(mousePos, justClicked);
            btnReserve.update(mousePos, justClicked);
        }


        window.clear(Config::BG_DARK);


        sf::RectangleShape sidebar({180.f, (float)Config::WINDOW_HEIGHT});
        sidebar.setFillColor(sf::Color(15, 15, 20));
        sidebar.setOutlineThickness(1.f);
        sidebar.setOutlineColor(Config::BORDER_COLOR);
        window.draw(sidebar);
        

        sf::Text title(font, Config::WINDOW_TITLE, 20);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(Config::NEON_CYAN);
        title.setPosition({200.f, 20.f});
        

        sf::Text glowTitle = title;
        glowTitle.setFillColor(sf::Color(0, 240, 255, 50));
        glowTitle.setPosition({202.f, 22.f});
        window.draw(glowTitle);
        window.draw(title);

        sf::RectangleShape hLine({(float)Config::WINDOW_WIDTH - 220.f, 2.f});
        hLine.setPosition({200.f, 55.f});
        hLine.setFillColor(Config::NEON_PURPLE);
        window.draw(hLine);


        for(auto& btn : navButtons) btn.draw(window);


        float contentX = 200.f;
        float contentY = 80.f;

        if (currentState == AppState::HOME) {
            sf::Text msg(font, "Welcome Console -> GUI System\n\nSelect an option from the menu.", 16);
            msg.setPosition({contentX, contentY});
            msg.setFillColor(Config::TEXT_PRIMARY);
            window.draw(msg);
        }
        else if (currentState == AppState::ADD_GUEST) {
            sf::Text hdr(font, "Add New Guest", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            
            inpName.draw(window);
            inpPhone.draw(window);
            inpEmail.draw(window);
            btnAddGuest.draw(window);
        }
        else if (currentState == AppState::VIEW_ROOMS) {
            sf::Text hdr(font, "Available Rooms", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            
            float y = contentY + 40.f;
            for(const auto& r : hotel.rooms) {
                if(y > Config::WINDOW_HEIGHT - 50) break;
                std::stringstream ss;
                ss << "Room " << r->number << " [" << r->type << "] - $" << r->price;
                if(!r->available) ss << " [OCCUPIED]";
                sf::Text line(font, ss.str(), 14);
                line.setPosition({contentX, y});
                line.setFillColor(r->available ? Config::NEON_GREEN : Config::NEON_RED);
                window.draw(line);
                y += 20.f;
            }
        }
        else if (currentState == AppState::RESERVATION) {
             sf::Text hdr(font, "New Reservation", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            
            resGuestId.draw(window);
            resRoomNum.draw(window);
            resCheckIn.draw(window);
            resCheckOut.draw(window);
            resNights.draw(window);
            chkBreakfast.draw(window);
            chkLunch.draw(window);
            chkDinner.draw(window);
            btnReserve.draw(window);
        }
        else if (currentState == AppState::GUESTS_LIST) {
             sf::Text hdr(font, "Guest Directory", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            float y = contentY + 40.f;
            for(const auto& g : hotel.guests) {
                 if(y > Config::WINDOW_HEIGHT - 50) break;
                 std::string s = "ID: " + std::to_string(g.id) + " | " + g.name + " | " + g.phone;
                 sf::Text t(font, s, 14);
                 t.setPosition({contentX, y});
                 t.setFillColor(Config::TEXT_PRIMARY);
                 window.draw(t);
                 y += 20.f;
            }
        }
        else if (currentState == AppState::RES_LIST) {
             sf::Text hdr(font, "Active Reservations", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            float y = contentY + 40.f;
            for(const auto& r : hotel.reservations) {
                 if(y > Config::WINDOW_HEIGHT - 50) break;
                 std::stringstream ss;
                 ss << "Res#" << r.id << " Guest:" << r.guestId << " Rm:" << r.roomNumber << " Amt:$" << r.totalAmount;
                 sf::Text t(font, ss.str(), 14);
                 t.setPosition({contentX, y});
                 t.setFillColor(Config::TEXT_PRIMARY);
                 window.draw(t);
                 y += 20.f;
            }
        }
        else if (currentState == AppState::STATS) {
            sf::Text hdr(font, "Hotel Statistics", 18);
            hdr.setFillColor(Config::NEON_MAGENTA);
            hdr.setPosition({contentX, contentY});
            window.draw(hdr);
            
            int occup = 0;
            for(auto& r : hotel.rooms) if(!r->available) occup++;
            
            std::stringstream ss;
            ss << "Total Rooms: " << hotel.rooms.size() << "\n\n";
            ss << "Occupied: " << occup << "\n\n";
            ss << "Available: " << (hotel.rooms.size() - occup) << "\n\n";
            ss << "Occupancy: " << (hotel.rooms.empty() ? 0 : (occup * 100 / hotel.rooms.size())) << "%\n\n";
            ss << "Total Revenue (Active): $" << 0.0;
            
            sf::Text st(font, ss.str(), 15);
            st.setPosition({contentX, contentY + 50.f});
            st.setFillColor(Config::NEON_CYAN);
            window.draw(st);
        }


        window.draw(statusText);

        window.display();
    }
    
    return 0;
}