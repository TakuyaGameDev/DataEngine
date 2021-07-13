#pragma once
#include <array>
#include "BaseScene.h"
#include "Geometory.h"

// 各種ﾓｰﾄﾞ
enum class MODE
{
    // 画像ﾃﾞｰﾀの編集ﾓｰﾄﾞ
    IMAGERESOURCE_EDITOR,
    // 当たり判定ﾎﾞｯｸｽの編集ﾓｰﾄﾞ
    COLLIDERBOX_EDITOR,
    MAX
};
// ﾓｰﾄﾞ毎にﾎﾟｼﾞｼｮﾝやｻｲｽﾞを格納させたいのでarrayで取っている
using ModeVec = std::array<Vector2I, static_cast<int>(MODE::MAX)>;

// ﾓｰﾄﾞｾﾚｸﾄｼｰﾝ
class Select :
    public BaseScene
{
public:
    Select();
    ~Select();
    // 更新
    UniqueScene Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse);
    // 初期化
    void Initialize(void);
    // ﾌｪｰｽﾞ毎の処理の初期化
    void InitializeUpdater(void);
    // ﾌｪｰｽﾞ毎の描画処理の初期化
    void InitializeDrawExcecuter(void);
    // 描画
    void Draw(void);

private:
    // 編集ﾓｰﾄﾞ表示のﾎﾟｼﾞｼｮﾝ
    ModeVec modePos_;
    // 編集ﾓｰﾄﾞ表示されているところにﾏｳｽﾎﾟｲﾝﾀが来た時に描画する透過された箱
    ModeVec boxSize_;
    // 現在選んでいるﾓｰﾄﾞ
    MODE nowMode_;
    // ｸﾘｯｸした時に選んでいたﾓｰﾄﾞ
    MODE decideMode_;
};

