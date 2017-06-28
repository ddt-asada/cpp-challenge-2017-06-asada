/*課題
 * vcfの文字列を一部追加するプログラム。
 * 作成日：2017年6月26日
 * 作成者：浅田　知嗣
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char** argv) {
	string vcard;									//元のvcardファイルより読み込んだ文字列を格納する文字列。
	string name;									//抽出した名前に当たる情報を格納する文字列。
	string fnameKana;								//姓のフリガナデータを抽出して格納する文字列。
	string lnameKana;								//名のフリガナデータを抽出して格納する文字列。
	string company;									//会社名を抽出して格納する文字列。
	string companyKana;								//会社名のフリガナデータを格納する文字列。
	string ltdTag = "株式会社";						//会社名を抜き出す時の抽出するタグとなる文字列。
	int ltdTagLen = ltdTag.length();				//会社名を操作する際に利用する文字列の長さを取得。
	string companyTag = "ORG:";						//会社名が格納された行を抽出するタグとなる文字列。
	int companyTagLen = companyTag.length();			//会社名を操作する際に利用する文字列の長さを取得する。
	string nameTag = "N:";							//名前が格納された行を抽出するタグとなる文字列。
	int nameTagLen = nameTag.length();			 	//名前を操作する際に利用する文字列の長さを取得する。
	string fnameTag = "X-PHONETIC-FIRST-NAME:";		//姓のフリガナデータを抽出するタグとなる文字列。
	int fnameTagLen = fnameTag.length();			//姓のフリガナデータを操作する際に利用する文字列の長さを取得する。
	string lnameTag = "X-PHONETIC-LAST-NAME:";		//名のフリガナデータを抽出するためのタグとなる文字列。
	int lnameTagLen = lnameTag.length();			//名のフリガナデータを操作する際に利用する文字列の長さを取得する。
	string noteTag = "NOTE:";						//ノートを抽出するためのタグとなる文字列。
	string companyKanaTag = "社名読み:";			//社名のフリガナデータより社名のフリガナデータを抽出するための文字列。
	int companyKanaTagLen = companyKanaTag.length();//社名のフリガナデータを操作する際に利用する文字列の長さを取得する。
	string companyKanaTake = "\\n";					//社名のフリガナデータより社名のフリガナデータを抽出するための文字列。
	int length;										//文字列を操作する際に利用する文字列の長さを格納する。
	int pos;										//文字列を操作する際に利用する文字列の添字を格納する。。

	ifstream vcf(argv[1]);

	//編集するためのデータを保管する一時的なテキストファイル。
	ofstream cpy("copy.txt");

	//編集元のファイルから編集したいデータのみを取り出して編集用のファイルを作る繰り返し。
	while(getline(vcf, vcard)) {

		//名前の行にたどり着いた時に名前を記憶させるための分岐。
		if(vcard.find(nameTag) == 0) {
			//名前が格納された行より名前の部分だけを抽出させる。
			name = vcard.substr(nameTagLen);

		//会社名の行にたどり着いたときに会社名を記憶させるための分岐。
		} else if(vcard.find(companyTag) == 0) {
			//会社名の抽出タグ以降の文字列を抽出する。
			company = vcard.substr(companyTagLen);
			for(pos = company.find(ltdTag); pos != -1; pos = company.find(ltdTag)) {
				//抽出した文字列から「株式会社が全て削除されるまで繰り返す。」を削除する。
				company.erase(pos, ltdTagLen);
			}
			//取り出したデータを連結して編集用のファイルに格納する。
			cpy <<"N:" << company <<')' <<name <<'\n';

		//姓の行にたどり着いたときにフリガナを記憶させる分岐。
		} else if(vcard.find(fnameTag) == 0) {
			//姓のフリガナデータを抽出する。
			fnameKana = vcard.substr(fnameTagLen);

		//名の行にたどり着いたときにフリガナを記憶させる分岐。
		} else if(vcard.find(lnameTag) == 0) {
			//名のフリガナデータのみを抽出する。
			lnameKana = vcard.substr(lnameTagLen);

		//ノートの行にたどり着いたときに社名のフリガナを記憶させる分岐。
		} else if(vcard.find(noteTag) == 0) {
			//ノートの中に社名のフリガナが含まれていたときに社名のフリガナを記憶させる。
			if(vcard.find(companyKanaTag) != string::npos) {
				//フリガナを抽出していく。
				pos = vcard.find(companyKanaTag) + companyKanaTagLen;
				//もしフリガナの前に空白文字があった場合はその分添え字を進める。
				if(vcard[pos] == ' ') {
					pos++;
				}
				companyKana = vcard.substr(pos);
				//改行文字までを削除するため改行文字の添え字を取得。
				length = companyKana.find(companyKanaTake);
				if(length > -1) {
					//改行文字から後ろを削除。
					companyKana.erase(length);
				}
			}

			//取り出したデータを連結して編集用のファイルに格納する。
			cpy <<"SORT-STRING:" <<companyKana <<' ' <<fnameKana <<' ' <<lnameKana <<'\n';
			//取り出したデータを連結して編集用のファイルに格納する。
			cpy <<"X-PHONETIC-FIRST-NAME:" <<fnameKana <<'\n';
			//取り出したデータを連結して編集用のファイルに格納する。
			cpy <<"X-PHONETIC-LAST-NAME:" <<companyKana <<"）" <<lnameKana <<'\n';

			companyKana = '\0';
			fnameKana = '\0';
			lnameKana = '\0';
		}
	}

	vcf.close();
	cpy.close();

	//編集元のファイルを開く。
	ifstream vcf2(argv[1]);
	//編集用のデータがはいったファイルを開く。
	ifstream cpy2("copy.txt");

	ofstream vch(argv[2]);

	//編集元のデータと編集用のデータを連結させていき、編集後のファイルを作成する繰り返し。
	while(getline(vcf2,vcard)) {
		//名前の行にたどり着いたときに編集用のデータを取り出す分岐。
		if(vcard.find(nameTag) == 0) {
			//編集用のデータから名前のデータを取り出す。
			getline(cpy2, name);
			//編集後のファイルに連結していく。
			vch <<name <<'\n';
			//編集用のデータから編集したソート文字列タグを取り出す。
			getline(cpy2, fnameKana);
			//取り出したデータを連結させていく。
			vch <<fnameKana <<'\n';
			//編集用のデータから編集した姓のフリガナデータを取り出す。
			getline(cpy2, fnameKana);
			//取り出したデータを連結させていく。
			vch <<fnameKana <<'\n';
			//編集用のデータから編集した名のフリガナデータを取り出す。
			getline(cpy2, lnameKana);
			//取り出したデータを連結させていく。
			vch <<lnameKana <<'\n';

		//姓またはフリガナの行でないときはそのままデータを取り出す分岐。
		} else if((vcard.find(lnameTag) > 0) && (vcard.find(fnameTag) > 0)){
			//元のデータから取り出したデータをそのまま格納する。
			vch <<vcard <<'\n';
		}
	}
}

