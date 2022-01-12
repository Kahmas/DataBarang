#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct Barang {
	int pk;
	char id[25];
	char name[25];
	char qty[25];
};

int getOption();
void checkDatabase(fstream &data);
void writeData(fstream &data, int posisi, Barang &inputBarang);
int getDataSize(fstream &data);
Barang readData(fstream &data, int posisi);
void addDataBarang(fstream &data);
void displayDataBarang(fstream &data);
void updateRecord(fstream &data);
void deleteRecord(fstream &data);

int main(){

	fstream data;
	
	checkDatabase(data);

	int pilihan = getOption();
	char is_continue;

	enum option{CREATE = 1, READ, UPDATE, DELETE, FINISH};

	while(pilihan != FINISH){
		
		switch(pilihan){
			case CREATE:
				cout << "\nMenambah data barang" << endl;
				addDataBarang(data);
				break;
			case READ:
				cout << "\nTampilkan data barang" << endl;
				displayDataBarang(data);
				break;
			case UPDATE:
				cout << "\nUbah data barang" << endl;
                displayDataBarang(data);
                updateRecord(data);
                displayDataBarang(data);
				break;
			case DELETE:
				cout << "\nHapus data barang" << endl;
				displayDataBarang(data);
                deleteRecord(data);
                displayDataBarang(data);
                break;
			default:
				cout << "\nPilihan tidak ditemukan!" << endl;
				break;
		}

		label_continue:

		cout << "Lanjutkan?(y/n) : ";
		cin >> is_continue;
		if ( (is_continue == 'y') | (is_continue == 'Y')){
			pilihan = getOption();
		} else if ((is_continue == 'n') | (is_continue == 'N')){
			break;
		} else {
			goto label_continue;
		}
	}

	//cout << "akhir dari program" << endl;

	cin.get();
	return 0;
}

void deleteRecord(fstream &data){
    int nomor, size, offset;
    Barang blankBarang, tempBarang;
    fstream tempData;

    size = getDataSize(data);
    // 1. pilih nomor
    cout << "Hapus nomor : ";
    cin >> nomor;
    // 2. diisi data kosong
    writeData(data, nomor, blankBarang);
    // 3. pindahkan data yang ada dari file bin, kalau ada dipindahkan ke file sementara
    tempData.open("temp.dat", ios::trunc | ios::out | ios::in | ios::binary);

    offset = 0;
    for(int i = 1; i <= size; i++){
        tempBarang = readData(data, i);

        if (tempBarang.name[i] != '\0'){
            writeData(tempData, i - offset, tempBarang);
        } else {
            offset++;
            //cout << "deleted item" << endl;
        }
    }
    // 4. pindahkan data dari file sementarake data bin
    size = getDataSize(tempData);
    data.close();
    data.open("data.bin", ios::trunc | ios::out | ios::binary);
    data.close();
    data.open("data.bin", ios::out | ios::in | ios::binary);

    for(int i = 1; i <= size; i++){
        tempBarang = readData(tempData, i);
        writeData(data, i, tempBarang);
    }
}

void updateRecord(fstream &data){
    int nomor;

    Barang updateBarang;
    cout << "\nPilih no : ";
    cin >> nomor;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    updateBarang = readData(data, nomor);

    cout << "\n\nPilihan data : " << endl;
    cout << "ID : " << updateBarang.id << endl;
    cout << "Name : " << updateBarang.name << endl;
    cout << "QTY : " << updateBarang.qty << endl;

    cout << "\nMerubah data: " << endl;
    cout << "ID: ";
    cin.get(updateBarang.id, 25);
    cin.ignore();
    cout << "Name: ";
    cin.get(updateBarang.name, 25);
    cin.ignore();
    cout << "QTY: ";
    cin.get(updateBarang.qty, 25);
    cin.ignore();

    writeData(data, nomor, updateBarang);
}

void checkDatabase(fstream &data){
	data.open("data.bin", ios::out | ios::in | ios::binary);

	// check file ada atau tidak
	if (data.is_open()){
		cout << "database ditemukan" << endl;
	} else {
		cout << "database tidak ditemukan, buat database baru" << endl;
		data.close();
		data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
	}
}

int getOption(){
	int input;
	//system("clear");
	system("cls");
	cout << "\nProgram CRUD data Barang" << endl;
	cout << "=============================" << endl;
	cout << "1. Tambah data barang" << endl;
	cout << "2. Tampilkan data barang" << endl;
	cout << "3. Ubah data barang" << endl;
	cout << "4. Hapus data barang" << endl;
	cout << "5. Selesai" << endl;
	cout << "=============================" << endl;
	cout << "pilih [1-5]? : ";
	cin >> input;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	return input;
}

void writeData(fstream &data, int posisi, Barang &inputBarang){
	data.seekp((posisi - 1)*sizeof(Barang), ios::beg);
	data.write(reinterpret_cast<char*>(&inputBarang),sizeof(Barang));
}

int getDataSize(fstream &data){
	int start, end;
	data.seekg(0,ios::beg);
	start = data.tellg();
	data.seekg(0,ios::end);
	end = data.tellg();
	return (end-start)/sizeof(Barang);
}

Barang readData(fstream &data, int posisi){
	Barang readBarang;
	data.seekg((posisi - 1)*sizeof(Barang),ios::beg);
	data.read(reinterpret_cast<char*>(&readBarang),sizeof(Barang));
	return readBarang;
}

void addDataBarang(fstream &data){
	
	Barang inputBarang, lastBarang;

	int size = getDataSize(data);
	
	cout << "ukuran data : " << size << endl;

	if(size == 0){
		inputBarang.pk = 1;
	} else {
		lastBarang = readData(data,size);
		cout << "pk = " << lastBarang.pk << endl;
		inputBarang.pk = lastBarang.pk + 1;
	}

	cout << "Name : ";
	cin.get(inputBarang.name, 25);
	cin.ignore();
	cout << "ID : ";
	cin.get(inputBarang.id, 25);
	cin.ignore();
	cout << "QTY : ";
	cin.get(inputBarang.qty, 25);
	cin.ignore();

	writeData(data,size+1,inputBarang);
}

void displayDataBarang(fstream &data){
	int size = getDataSize(data);
	Barang showBarang;
	cout << "no.\tpk.\tID.\t\tName.\t\tQTY." << endl;
	for(int i = 1; i <= size; i++){
		showBarang = readData(data,i);
		cout << i << "\t";
		cout << showBarang.pk << "\t";
		cout << showBarang.id << "\t\t";
		cout << showBarang.name << "\t\t";
		cout << showBarang.qty << endl;
	}
}