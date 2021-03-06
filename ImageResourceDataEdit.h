#pragma once
#include <array>
#include <map>
#include <vector>
#include <functional>
#include "BaseScene.h"
#include "Geometory.h"

// 【決定】なのか【戻る】なのか
enum class OperationProcess
{
    // 決定
    DECISION,
    // 戻る
    RETURN,
    MAX
};

using ProcessVec = std::array<Vector2I, static_cast<int>(OperationProcess::MAX)>;

// ImageDataを編集するｼｰﾝ 
class ImageResourceDataEdit :
    public BaseScene
{
public:
    ImageResourceDataEdit();
    ~ImageResourceDataEdit();

    // 更新
    UniqueScene Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse);
    // 初期化
    void Initialize(void);
    // ﾌｪｰｽﾞ毎の処理の初期化
    void InitializeUpdater(void);
    // ﾌｪｰｽﾞ毎の描画処理の初期化
    void InitializeDrawExcecuter(void);
    // 描画
    void Draw(void);

    // Phase::INPUTDIRECTORYで使用-----------------
    // ﾃﾞｨﾚｸﾄﾘ名の入力
    void InputDirectoryName(void);
    // ｵﾌﾞｼﾞｪｸﾄ名の入力
    void InputObjectName(void);
    // --------------------------------------------

    // Phase::DATAEDITで使用-----------------------
    // 最大ﾌﾚｰﾑの入力
    void InputMaxFrame(void);
    // ﾙｰﾌﾟﾌﾗｸﾞの入力(0か1でtrueかfalseの判断)
    void InputLoopFlag(void);
    // 1ｺﾏにかかる時間の入力
    void InputDuration(void);
private:
    // 【決定】と【戻る】のﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝ
    ProcessVec processPos_;
    // 【決定】と【戻る】のﾎﾞﾀﾝﾎﾞｯｸｽｻｲｽﾞ
    ProcessVec boxSize_;
    // 今現在の選択事項(【決定】か【戻る】のどちらか)
    OperationProcess nowOperation_;
    // ｸﾘｯｸ時の選択事項(【決定】か【戻る】のどちらか)
    OperationProcess excecuteProcess_;
    // 入力欄のﾎﾟｼﾞｼｮﾝ
    Vector2I inputBoxPos_;
    // 入力欄のｻｲｽﾞ
    Vector2I inputBoxSize_;
    // 【入力】という文字のﾎﾟｼﾞｼｮﾝ
    Vector2I inputStringPos_;
    // 【入力】という文字のｻｲｽﾞ
    Vector2I inputStringSize_;
    // ｱﾆﾒｰｼｮﾝ名が表示されているﾎﾟｼﾞｼｮﾝ
    std::vector<Vector2I> animNamePos_;
    // 表示されているｱﾆﾒｰｼｮﾝ名全体のｻｲｽﾞ(ﾌｧｲﾙ名が12文字だったら12*40になる)
    std::vector<Vector2I> animNameSize_;
    // 現在のﾌｪｰｽﾞ
    Phase nowPhase_;
    // ﾌｪｰｽﾞ毎の処理を記述
    std::map<Phase,std::function<void(const std::shared_ptr<Mouse>& mouse)>> updater_;
    // ﾌｪｰｽﾞ毎の描画処理
    std::map<Phase, std::function<void()>> drawExcecuter_;
    // 入力ﾃﾞｨﾚｸﾄﾘ名
    char inputDirectoryName_[20];
    // 入力ｵﾌﾞｼﾞｪｸﾄ名
    char inputObjectName_[20];
    // 入力した最大ﾌﾚｰﾑ数
    char inputMaxFrame_[20];
    // 入力したloopFlag(ｱﾆﾒｰｼｮﾝをﾙｰﾌﾟ再生するのか(true)しないのか(false))
    char inputLoopFlag_[20];
    // 入力したduration(1ｺﾏにかかる時間)
    char inputDuration_[20];
    // 入力した際に生成されるﾊﾝﾄﾞﾙ
    int inputHandle_[4];
    // 入力段階毎の関数ﾎﾟｲﾝﾀ
    void (ImageResourceDataEdit::* inputFunc_)(void);
    // 【入力】というﾎﾞﾀﾝの上にﾏｳｽﾎﾟｲﾝﾀが来たらtrueになるﾌﾗｸﾞ
    bool onInputDirectoryButton_;
    // ﾃﾞｨﾚｸﾄﾘ名の入力可能のﾌﾗｸﾞ
    bool inputDirectoryFlag_;
    // ﾏｳｽﾎﾟｲﾝﾀが乗ったﾌｧｲﾙのﾅﾝﾊﾞｰ
    int animNoOnMouse_;
    // 選択したﾌｧｲﾙ名ﾅﾝﾊﾞｰ(可変長配列で作られているのでこの番号で選択ﾌｧｲﾙにｱｸｾｽ)
    int selectAnimNo_;
    // ﾃﾞｰﾀをｾｰﾌﾞしたことを表すﾌﾗｸﾞ
    bool saveCmpFlag_;
};

