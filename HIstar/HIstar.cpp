#include "HIstar.h"


HIstar::HIstar(QWidget* parent)
	: QMainWindow(parent)

{
	ui.setupUi(this);
	connect(ui.search, SIGNAL(clicked()), this, SLOT(search()));// 조회하기버튼
	connect(ui.login_1, SIGNAL(clicked()), this, SLOT(login()));// 로그인 버튼
	connect(ui.refresh_2, SIGNAL(clicked()), this, SLOT(refresh()));// 새로고침 버튼
	//connect(ui.add_2, SIGNAL(clicked()), this, SLOT(add_item()));
	connect(ui.order_2, SIGNAL(clicked()), this, SLOT(order()));// 주문하기 버튼
	//connect(ui.remove_2, SIGNAL(clicked()), this, SLOT(remove()));
	ui.spinBox->setRange(1, 100);


}
// 로그인
void HIstar::login() {

	QString id = ui.textEdit->toPlainText();
	QString pw = ui.textEdit_2->toPlainText();
	if (id == "root")
	{
		if (pw == "1234")
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (pw != "1234")
		{
			// 비밀번호 틀릴시
			QMessageBox::information(this, "error", "Invalid PW. Please try again.");
		}
	}
	else
	{
		// 아이디 틀릴시
		QMessageBox::information(this, "error", "Invalid ID. Please try again.");
	}
	ui.refresh_2->setEnabled(false);



}


// 조회하기

void HIstar::search() {
	{
		MYSQL conn;
		MYSQL* conn_result;
		MYSQL_RES* result;
		MYSQL_ROW row;
		int query_state;

		mysql_init(&conn);

		conn_result = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (const char*)NULL, 0);
		
		// 컬럼 가져오기
		
		const char* query = "SELECT * FROM ingredient";
		query_state = mysql_query(conn_result, query);

		result = mysql_store_result(conn_result);
		int column_cnt = mysql_num_fields(result);

		while (row = mysql_fetch_row(result))
		{
			int row_count = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(row_count);
			ui.tableWidget->resizeColumnsToContents();//객체요소들의 길이에 테이블위젯헤더의 길이를 맞추는 코드
			ui.tableWidget->horizontalHeader()->setStretchLastSection(true);//헤더(맨위의 열)를 위젯안에 꽉 채우는? 그런 코드
			ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//속에 들어있는 객체를 수정하지 못하게 하기위한 코드
			ui.comboBox->addItem(row[0]); // 콤보박스에 아이템 추가하는 부분
			for (int i = 0; i < column_cnt; i++)
			{
				qDebug() << row[i] << "\t";
				ui.tableWidget->setItem(row_count, 0, new QTableWidgetItem((row[0])));
				ui.tableWidget->setItem(row_count, 1, new QTableWidgetItem((row[1])));
				ui.tableWidget->setItem(row_count, 2, new QTableWidgetItem((row[2])));

			}
			//cout << endl;
		}

		mysql_free_result(result);
		mysql_close(&conn);

	}
	ui.search->setEnabled(false);
	ui.refresh_2->setEnabled(true);
}

// 새로고침
void HIstar::refresh() {
	MYSQL conn;
	MYSQL* conn_result;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_state;

	// 테이블 지우기
	while (ui.tableWidget->rowCount() > 0) ui.tableWidget->removeRow(0);

	mysql_init(&conn);

	conn_result = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (const char*)NULL, 0);

	const char* query = "SELECT * FROM ingredient";
	query_state = mysql_query(conn_result, query);

	result = mysql_store_result(conn_result);
	int column_cnt = mysql_num_fields(result);

	// 데이터 테이블에 넣기
	while (row = mysql_fetch_row(result))
	{
		int row_count = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row_count);
		//ui.tableWidget->resizeColumnsToContents();//객체요소들의 길이에 테이블위젯헤더의 길이를 맞추는 코드
		//ui.tableWidget->horizontalHeader()->setStretchLastSection(true);//헤더(맨위의 열)를 위젯안에 꽉 채우는? 그런 코드
		//ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//속에 들어있는 객체를 수정하지 못하게 하기위한 코드
		for (int i = 0; i < column_cnt; i++)
		{
			qDebug() << row[i] << "\t";
			ui.tableWidget->setItem(row_count, 0, new QTableWidgetItem((row[0])));
			ui.tableWidget->setItem(row_count, 1, new QTableWidgetItem((row[1])));
			ui.tableWidget->setItem(row_count, 2, new QTableWidgetItem((row[2])));

		}
		//cout << endl;
	}
	ui.order_2->setEnabled(true);
	mysql_free_result(result);
	mysql_close(&conn);

}

void HIstar::order()//발주하기
{
	// 발주할때 업데이트 하는 개수 
	each111 = 0;

	// 가격
	int price = 0;

	
	//qDebug() << each111;
	//qDebug() << item;
	//qDebug() << each;


	// 발주 및 구매기록에 들어가는 날짜
	time_t timer;
	struct tm* t;
	timer = time(NULL);
	t = localtime(&timer);
	QString time = QString::number(t->tm_year + 1900) + "-" + QString::number(t->tm_mon + 1) + "-" + QString::number(t->tm_mday);  //현재날짜



	MYSQL conn;
	MYSQL* conn_result;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_state;

	mysql_init(&conn);

	conn_result = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (const char*)NULL, 0);

	item = ui.comboBox->currentText();
	string tempitem = item.toStdString();
	int each = ui.spinBox->value();


	////////////총재고량가져오기



	int num = ui.spinBox->value();
	/*QString str1 = QString("SELECT remain_quantity FROM ingredient WHERE ingredient_name = '%1'").arg(item);
	char strChar1[10000];
	memcpy(&strChar1, str1.toUtf8().data(), (size_t)str1.toUtf8().size());
	const char* query1 = strChar1;*/
	string a = "SELECT remain_quantity FROM ingredient WHERE ingredient_name = '"+tempitem+"'";
	mysql_query(conn_result, a.c_str());
	result = mysql_store_result(conn_result);
	row = mysql_fetch_row(result);
	each111 = num + atoi(row[0]);
	qDebug() << "each111 ====== ? "<<each111;




	//int row_count1 = ui.tableWidget_2->rowCount();
	//ui.tableWidget_2->setItem(row_count1, 0, new QTableWidgetItem(item));
	//ui.tableWidget_2->setItem(row_count1, 1, new QTableWidgetItem(each));
	//ui.tableWidget_2->setItem(row_count1, 2, new QTableWidgetItem(each));
	

	// 발주목록 테이블에 넣기 

	ui.tableWidget_2->resizeColumnsToContents();//객체요소들의 길이에 테이블위젯헤더의 길이를 맞추는 코드
	ui.tableWidget_2->horizontalHeader()->setStretchLastSection(true);//헤더(맨위의 열)를 위젯안에 꽉 채우는? 그런 코드
	ui.tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//속에 들어있는 객체를 수정하지 못하게 하기위한 코드

	ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount()); // 행 추가
	ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 0, new QTableWidgetItem(item));
	ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 1, new QTableWidgetItem(QString::number(each)));
	ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 2, new QTableWidgetItem(time));



	// 발주 넣는 쿼리문
	QString str = QString("UPDATE ingredient SET remain_quantity = %1 WHERE ingredient_name = '%2'").arg(each111).arg(item);

	char strChar[10000];
	memcpy(&strChar, str.toUtf8().data(), (size_t)str.toUtf8().size());

	const char* query = strChar;
	mysql_query(conn_result, query);

	// 구매기록 넣는 쿼리문

	// 구매기록에 넣는 가격
	if (item == "beans")
	{
		price = 200;
	}
	else if (item == "water")
	{
		price = 0;
	}
	else if (item == "ice")
	{
		price = 0;
	}
	else if (item == "milk")
	{
		price = 300;
	}
	else if (item == "condensed_milk")
	{
		price = 500;
	}
	else if (item == "whipped_cream")
	{
		price = 500;
	}
	else if (item == "whipping_cream")
	{
		price = 500;
	}
	else if (item == "caramel_syrup")
	{
		price = 500;
	}
	else if (item == "vanilla_syrup")
	{
		price = 500;
	}
	else if (item == "dolce_syrup")
	{
		price = 500;
	}
	else if (item == "grape_syrup")
	{
		price = 500;
	}
	else if (item == "peach_syrup")
	{
		price = 500;
	}
	else if (item == "mocha_powder")
	{
		price = 500;
	}
	else if (item == "cocoa_powder")
	{
		price = 500;
	}
	else if (item == "green_tea_powder")
	{
		price = 500;
	}
	else if (item == "strwberry")
	{
		price = 800;
	}
	else if (item == "mango")
	{
		price = 800;
	}
	else if (item == "peanut")
	{
		price = 300;
	}
	else if (item == "banana")
	{
		price = 800;
	}
	else if (item == "chocolate_chip")
	{
		price = 600;
	}
	else if (item == "java_chip")
	{
		price = 700;
	}
	else if (item == "star_candy")
	{
		price = 300;
	}
	else if (item == "vanilla_bean_ice_cream")
	{
		price = 500;
	}
	else if (item == "yogurt_powder")
	{
		price = 400;
	}
	else if (item == "mint_chocolate")
	{
		price = 100;
	}
	else if (item == "mugwort_rice_cake")
	{
		price = 700;
	}

	if (each111 > 500)
	{
		// 재료가 500개이상일때 주문시 주문은 되지만 그후에 버튼 잠김
		QMessageBox::information(this, "warning", "We don't order now because we're full of ingredients.");
		ui.order_2->setEnabled(false);
	}

	QString str2 = QString("INSERT INTO buy(name, quantity, date, price) VALUES('%1', %2, '%3', %4)").arg(item).arg(each).arg(time).arg(price);
	

	char strChar2[10000];
	memcpy(&strChar2, str2.toUtf8().data(), (size_t)str2.toUtf8().size());

	const char* query2 = strChar2;
	mysql_query(conn_result, query2);
	qDebug() << str2;



	mysql_close(&conn);
}

// 발주시 재고의 총량 가져오기
void HIstar::add_item()
{
	// 콤보박스에 있는 아이템을 받아오는 코드
	QString item1 = ui.comboBox->currentText();

	//스핀박스에 있는 값을 받아오는 코드
	int num = ui.spinBox->value();


	MYSQL conn;
	MYSQL* conn_result;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_state;

	//select 
	mysql_init(&conn);

	conn_result = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (const char*)NULL, 0);
	

	// 총재고량을 가져오는 코드
	QString str1 = QString("SELECT remain_quantity FROM ingredient WHERE ingredient_name = '%1'").arg(item);
	char strChar1[1000];
	memcpy(&strChar1, str1.toUtf8().data(), (size_t)str1.toUtf8().size());


	const char* query1 = strChar1;
	mysql_query(conn_result, query1);

	result = mysql_store_result(conn_result);
	row = mysql_fetch_row(result);

	//QString fu = QString(row[2]); 
	qDebug() << result;
	qDebug() << item1;	//선택된 콤보박스의 재료이름
	qDebug() << row[0];	//총재고량
	qDebug() << num;
	each111 = num + atoi(row[0]);
	qDebug() << "each ::    "<<each111;



	mysql_close(&conn);
}
