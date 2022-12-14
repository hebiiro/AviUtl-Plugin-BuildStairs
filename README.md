# AviUtl プラグイン - アイテムずらし

タイムラインのアイテムをずらしたり整列したりします。
[最新バージョンをダウンロード](../../releases/latest/)

## 導入方法

以下のファイルを AviUtl の Plugins フォルダに入れてください。
* BuildStairs.auf
* BuildStairs (フォルダ) (ボイスが必要な場合のみ)

## 使用方法

1. メニューの「表示」->「アイテムずらし」を選択してウィンドウを表示します。
1. タイムラインのアイテムをずらしたい順に選択します。
1. アイテムずらしウィンドウの「アイテムをずらす」ボタンを押します。

* ずれる量は、選択した順に 0 倍、1 倍、2 倍、3 倍・・・と増えていきます。

## 注意事項

* ずらした先にすでにアイテムが存在する場合はエラーメッセージが表示され失敗します。

## ボイス

[WavPlayer](../../../WavPlayer/) を導入している場合は BuildStairs フォルダ内の wav ファイルが再生されます。<br>
オフにしたい場合は BuildStairs フォルダを削除してください。

## 更新履歴

* 2.2.0 - 2023/01/07 音を外部プロセスで鳴らすように変更
* 2.1.0 - 2022/09/14 中間点を削除する機能を追加
* 2.0.0 - 2022/08/11 現在位置を基準にしてアイテムを整列する機能を追加
* 1.1.0 - 2022/08/09 開始フレームを揃える機能を追加
* 1.0.2 - 2022/08/08 中間点を持つアイテムに対応
* 1.0.1 - 2022/08/07 開始フレームが 0 未満にならないように修正
* 1.0.0 - 2022/08/07 初版

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r42 https://scrapbox.io/ePi5131/patch.aul

## クレジット

* Microsoft Research Detours Package https://github.com/microsoft/Detours
* aviutl_exedit_sdk https://github.com/ePi5131/aviutl_exedit_sdk
* Common Library https://github.com/hebiiro/Common-Library
* COEIROINK:エリーミア https://coeiroink.com/
* エリーミア（あらつち） https://artt.birchgame.org/blog/eliemia/

## 作成者情報
 
* 作成者 - 蛇色 (へびいろ)
* GitHub - https://github.com/hebiiro
* Twitter - https://twitter.com/io_hebiiro

## 免責事項

この作成物および同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、私と私の関係者および私の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。
