#pragma once
#include <map>
#include <array>
#include <string>
#include <functional>
#include <vector>
#include "BaseScene.h"
#include "Geometory.h"

class ColliderBoxManager;

enum class CLICK
{
    RIGHT,
    LEFT,
    MAX
};

// 当たり判定矩形の編集ｼｰﾝ
class ColliderBoxEdit :
    public BaseScene
{
public:
    ColliderBoxEdit();
    ~ColliderBoxEdit();
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

    // ｵﾌﾞｼﾞｪｸﾄ名を入力する処理
    void InputObjectName(void);
    // ｱﾆﾒｰｼｮﾝ名を入力する処理
    void InputAnimationName(void);
    // ｱﾆﾒｰｼｮﾝのｺﾏを指定する処理
    void SelectFrame(const CLICK& click,const Vector2I& mPoint);
    // 当たり判定矩形の編集
    void BoxEdit(const CLICK& click, const Vector2I& mPoint);

private:
    // 入力ｵﾌﾞｼﾞｪｸﾄ名
    char inputObjectName_[20];
    // 入力ｱﾆﾒｰｼｮﾝ名
    char inputAnimationName_[20];
    // ｵﾌﾞｼﾞｪｸﾄ名を入力するﾎﾞｯｸｽのﾎﾟｼﾞｼｮﾝ
    Vector2I objectNameBoxPos_;
    // ｵﾌﾞｼﾞｪｸﾄ名を入力するﾎﾞｯｸｽのｻｲｽﾞ
    Vector2I objectNameBoxSize_;
    // ｱﾆﾒｰｼｮﾝ名を入力するﾎﾞｯｸｽのﾎﾟｼﾞｼｮﾝ
    Vector2I animationNameBoxPos_;
    // ｱﾆﾒｰｼｮﾝ名を入力するﾎﾞｯｸｽのｻｲｽﾞ
    Vector2I animationNameBoxSize_;
    // ﾌﾚｰﾑ数を表示するﾎﾟｼﾞｼｮﾝ
    // ﾌﾚｰﾑ数だけsizeがあるので可変にしている
    std::vector<std::pair<Vector2I,int>> frameNumBox_;
    // ﾌﾚｰﾑを表示するﾎﾞｯｸｽのｻｲｽﾞ
    Vector2I frameNumBoxSize_;
    // 矩形ﾀｲﾌﾟ変更を行うﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝ
    std::array<Vector2I, 2> typeChangerPos_;
    // 矩形ﾀｲﾌﾟ変更を行うﾎﾞﾀﾝのｻｲｽﾞ
    Vector2I typeChangerBoxSize_;
    // ｾｰﾌﾞﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝ
    Vector2I saveButtonPos_;
    // ｾｰﾌﾞﾎﾞﾀﾝのｻｲｽﾞ
    Vector2I saveButtonSize_;
    // ﾃﾞｰﾀが無かった時の画像ﾃﾞｰﾀ編集に移行するﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝ
    Vector2I goImageDataButtonPos_;
    // 画像ﾃﾞｰﾀ編集に移行するﾎﾞﾀﾝのｻｲｽﾞ
    Vector2I goImageDataButtonSize_;
    // ｱﾌﾟﾘｹｰｼｮﾝを終了させるﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝ
    Vector2I exitAppButtonPos_;
    // ｱﾌﾟﾘｹｰｼｮﾝを終了させるﾎﾞﾀﾝのｻｲｽﾞ
    Vector2I exitAppButtonSize_;
    // ｾﾚｸﾄ画面に戻るﾎﾞﾀﾝの上にﾏｳｽﾎﾟｲﾝﾀが来たﾌﾗｸﾞ
    bool isOnReturn_;
    // ｱﾌﾟﾘｹｰｼｮﾝを終了させるﾎﾞﾀﾝの上にﾏｳｽﾎﾟｲﾝﾀが来たﾌﾗｸﾞ
    bool isOnExitApp_;

    // 編集対象画像のﾎﾟｼﾞｼｮﾝ
    Vector2I objImagePos_;

    // ﾏｳｽﾎﾟｲﾝﾀが乗っているﾌﾚｰﾑ
    int onPointFrame_;
    // 決定(左ｸﾘｯｸ)後のﾌﾚｰﾑ数
    int selectedFrame_;
    // 現在の処理段階
    Phase nowPhase_;
    // 左右ｸﾘｯｸ変数
    CLICK clickDir_;
    // ﾏｳｽﾎﾟｲﾝﾀ
    Vector2I mPoint_;
    // 入力ﾀｰﾝを表す関数ﾎﾟｲﾝﾀ
    void(ColliderBoxEdit::* inputFunc_)(void);
    // 編集処理を表す関数ﾎﾟｲﾝﾀ
    void(ColliderBoxEdit::* editFunc_)(const CLICK& click, const Vector2I& mPoint);
    // ｼｰﾝの中での状態遷移関数をstd::functionでまとめた
    std::map<Phase, std::function<void(const std::shared_ptr<Mouse>& mouse)>> updater_;
    // 描画遷移関数をstd::functionでまとめた
    std::map<Phase, std::function<void(void)>> drawExcecuter_;
    // ﾌｧｲﾙが見つかった際にはtrue
    bool isFindFile_;

    // 当たり判定ﾎﾞｯｸｽの生成等を担うManager
    ColliderBoxManager* boxManager_;

    // ﾎﾞｯｸｽﾀｲﾌﾟ毎の色
    unsigned int typeColor_;
    // ﾎﾞｯｸｽﾀｲﾌﾟ
    bool typeFlag_;
    // ｾｰﾌﾞしたﾌﾗｸﾞ
    std::vector<bool> saveFlag_;
    // 再度ﾘｽﾀｰﾄする際にﾌﾗｸﾞ等をﾘｾｯﾄ
    void ResetData(void);
};

