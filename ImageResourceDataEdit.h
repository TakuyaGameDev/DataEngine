#pragma once
#include <array>
#include <map>
#include <vector>
#include <functional>
#include "BaseScene.h"
#include "Geometory.h"

// yŒˆ’èz‚È‚Ì‚©y–ß‚éz‚È‚Ì‚©
enum class OperationProcess
{
    // Œˆ’è
    DECISION,
    // –ß‚é
    RETURN,
    MAX
};

using ProcessVec = std::array<Vector2I, static_cast<int>(OperationProcess::MAX)>;

// ImageData‚ğ•ÒW‚·‚é¼°İ 
class ImageResourceDataEdit :
    public BaseScene
{
public:
    ImageResourceDataEdit();
    ~ImageResourceDataEdit();

    // XV
    UniqueScene Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse);
    // ‰Šú‰»
    void Initialize(void);
    // Ìª°½Ş–ˆ‚Ìˆ—‚Ì‰Šú‰»
    void InitializeUpdater(void);
    // Ìª°½Ş–ˆ‚Ì•`‰æˆ—‚Ì‰Šú‰»
    void InitializeDrawExcecuter(void);
    // •`‰æ
    void Draw(void);

    // Phase::INPUTDIRECTORY‚Åg—p-----------------
    // ÃŞ¨Ú¸ÄØ–¼‚Ì“ü—Í
    void InputDirectoryName(void);
    // µÌŞ¼Şª¸Ä–¼‚Ì“ü—Í
    void InputObjectName(void);
    // --------------------------------------------

    // Phase::DATAEDIT‚Åg—p-----------------------
    // Å‘åÌÚ°Ñ‚Ì“ü—Í
    void InputMaxFrame(void);
    // Ù°ÌßÌ×¸Ş‚Ì“ü—Í(0‚©1‚Åtrue‚©false‚Ì”»’f)
    void InputLoopFlag(void);
    // 1ºÏ‚É‚©‚©‚éŠÔ‚Ì“ü—Í
    void InputDuration(void);
private:
    // yŒˆ’èz‚Æy–ß‚éz‚ÌÎŞÀİ‚ÌÎß¼Ş¼®İ
    ProcessVec processPos_;
    // yŒˆ’èz‚Æy–ß‚éz‚ÌÎŞÀİÎŞ¯¸½»²½Ş
    ProcessVec boxSize_;
    // ¡Œ»İ‚Ì‘I‘ğ–€(yŒˆ’èz‚©y–ß‚éz‚Ì‚Ç‚¿‚ç‚©)
    OperationProcess nowOperation_;
    // ¸Ø¯¸‚Ì‘I‘ğ–€(yŒˆ’èz‚©y–ß‚éz‚Ì‚Ç‚¿‚ç‚©)
    OperationProcess excecuteProcess_;
    // “ü—Í—“‚ÌÎß¼Ş¼®İ
    Vector2I inputBoxPos_;
    // “ü—Í—“‚Ì»²½Ş
    Vector2I inputBoxSize_;
    // y“ü—Íz‚Æ‚¢‚¤•¶š‚ÌÎß¼Ş¼®İ
    Vector2I inputStringPos_;
    // y“ü—Íz‚Æ‚¢‚¤•¶š‚Ì»²½Ş
    Vector2I inputStringSize_;
    // ±ÆÒ°¼®İ–¼‚ª•\¦‚³‚ê‚Ä‚¢‚éÎß¼Ş¼®İ
    std::vector<Vector2I> animNamePos_;
    // •\¦‚³‚ê‚Ä‚¢‚é±ÆÒ°¼®İ–¼‘S‘Ì‚Ì»²½Ş(Ì§²Ù–¼‚ª12•¶š‚¾‚Á‚½‚ç12*40‚É‚È‚é)
    std::vector<Vector2I> animNameSize_;
    // Œ»İ‚ÌÌª°½Ş
    Phase nowPhase_;
    // Ìª°½Ş–ˆ‚Ìˆ—‚ğ‹Lq
    std::map<Phase,std::function<void(const std::shared_ptr<Mouse>& mouse)>> updater_;
    // Ìª°½Ş–ˆ‚Ì•`‰æˆ—
    std::map<Phase, std::function<void()>> drawExcecuter_;
    // “ü—ÍÃŞ¨Ú¸ÄØ–¼
    char inputDirectoryName_[20];
    // “ü—ÍµÌŞ¼Şª¸Ä–¼
    char inputObjectName_[20];
    // “ü—Í‚µ‚½Å‘åÌÚ°Ñ”
    char inputMaxFrame_[20];
    // “ü—Í‚µ‚½loopFlag(±ÆÒ°¼®İ‚ğÙ°ÌßÄ¶‚·‚é‚Ì‚©(true)‚µ‚È‚¢‚Ì‚©(false))
    char inputLoopFlag_[20];
    // “ü—Í‚µ‚½duration(1ºÏ‚É‚©‚©‚éŠÔ)
    char inputDuration_[20];
    // “ü—Í‚µ‚½Û‚É¶¬‚³‚ê‚éÊİÄŞÙ
    int inputHandle_[4];
    // “ü—Í’iŠK–ˆ‚ÌŠÖ”Îß²İÀ
    void (ImageResourceDataEdit::* inputFunc_)(void);
    // y“ü—Íz‚Æ‚¢‚¤ÎŞÀİ‚Ìã‚ÉÏ³½Îß²İÀ‚ª—ˆ‚½‚çtrue‚É‚È‚éÌ×¸Ş
    bool onInputDirectoryButton_;
    // ÃŞ¨Ú¸ÄØ–¼‚Ì“ü—Í‰Â”\‚ÌÌ×¸Ş
    bool inputDirectoryFlag_;
    // Ï³½Îß²İÀ‚ªæ‚Á‚½Ì§²Ù‚ÌÅİÊŞ°
    int animNoOnMouse_;
    // ‘I‘ğ‚µ‚½Ì§²Ù–¼ÅİÊŞ°(‰Â•Ï’·”z—ñ‚Åì‚ç‚ê‚Ä‚¢‚é‚Ì‚Å‚±‚Ì”Ô†‚Å‘I‘ğÌ§²Ù‚É±¸¾½)
    int selectAnimNo_;
    // ÃŞ°À‚ğ¾°ÌŞ‚µ‚½‚±‚Æ‚ğ•\‚·Ì×¸Ş
    bool saveCmpFlag_;
};

