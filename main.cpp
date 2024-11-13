#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <regex>
#include <sstream>
double get_rand() {
    double val = (rand()%100+10)/25.;
    while (val==0) {val = (rand()%100+10)/25.;}
    return val;
}

int Дней_В_Месяце(int месяц, int год) {
        switch (месяц) {
            case 2:
                if (год%4 == 0 && (год%100 != 0 || год%400 == 0)) {
                    return 29;
                }
                else {return 28;}
            case 4:
            case 6:
            case 9:
            case 11:
                return 30;
            default:
                return 31;
        }
    }



struct fio {
    std::string Фамилия;
    std::string Имя;
    std::string Отчество;
};



class Date {
private:
    unsigned int Год;
    unsigned int Месяц;
    unsigned int День;

    
public:
    
    Date(unsigned int Год, unsigned int Месяц, unsigned int День) {
        this->Год = Год;
        this->Месяц = Месяц;
        this->День = День;
    }
    Date(std::string date) {
        std::regex R(R"((\d*)\.(\d*)\.(\d*))");
        std::smatch matches;
        if (std::regex_search(date, matches, R)) {
            День = std::stoi(matches[1].str());
            Месяц = std::stoi(matches[2].str());
            Год = std::stoi(matches[3].str());
        }
    }
    Date(Date& date) {
        Год = date.Год;
        Месяц = date.Месяц;
        День = date.День;
    }

    bool friend operator<(Date& left ,Date& right) {
        return (
            left.GetYear()<right.GetYear() or
            (left.GetYear()==right.GetYear() and left.GetMonth()<right.GetMonth()) or
            (left.GetYear()==right.GetYear() and left.GetMonth()==right.GetMonth() and left.GetDay()<right.GetDay())
        );
    }
    bool friend operator>(Date& left ,Date& right) {
        return (
            left.GetYear()>right.GetYear() or
            (left.GetYear()==right.GetYear() and left.GetMonth()>right.GetMonth()) or
            (left.GetYear()==right.GetYear() and left.GetMonth()==right.GetMonth() and left.GetDay()>right.GetDay())
        );
    }
    bool friend operator==(Date& left,Date& right) {
        return (left.GetDay()==right.GetDay() && left.GetMonth()==right.GetMonth() && left.GetYear()==right.GetYear());
    }

    Date friend operator+(Date& left, Date& right) {
        
        int Year = left.GetYear()+right.GetYear();
        int Month = left.GetMonth();

        int Days = left.GetDay()+right.GetDay();
        for (int Months = right.GetMonth(); Months>0; Months--) {
            Days += Дней_В_Месяце(Months, Year);
        }

        while (Days>Дней_В_Месяце(Month, Year)) {
            Days -= Дней_В_Месяце(Month, Year);
            Month++;
            if (Month>12) {
                Month = 1;
                Year++;
            } 
        }

        return Date(Year, Month, Days);
    }
    Date friend operator+(Date& left, int Days) {
        int days = left.GetDay()+Days;
        int month = left.GetMonth();
        int year = left.GetYear();
        while (days>Дней_В_Месяце(month, year)) {
            days -= Дней_В_Месяце(month, year);
            month++;
            if (month>12) {
                month = 1;
                year++;
            } 
        }

        return Date(year, month, days);

    }
    std::string GetDate() {
        std::stringstream ss; 
        ss << День << '.' << Месяц << '.' << Год;
        return ss.str();
    }
    unsigned int GetYear() {return Год;}
    unsigned int GetMonth() {return Месяц;}
    unsigned int GetDay() {return День;}
    unsigned int GetDays() {
        int y = Год;
        int m = Месяц;
        int d = День;
        while (y>=0) {
            for (m;m>0;m--) {
                d+=Дней_В_Месяце(m, y);
            }
            y--;
            m=12;
        }
        return d;
    }
    int Get_Difference(Date& right) {
        return GetDays()-right.GetDays();
    }
};

class Payment {
private:
     fio ФИО;
     int Оклад;
     double Надбавка;
     Date Дата;
     double Налог = 13;
     unsigned int Отработанных_Дней;
     unsigned int Рабочих_Дней;
     unsigned int Начисленная_сумма;
     unsigned int Удержанные_суммы;
public:
    Payment(fio фио, int оклад, double надбавка, Date дата, unsigned int отработанных_Дней, unsigned int рабочих_Дней): ФИО(фио), Оклад(оклад), Дата(дата), Надбавка(надбавка),
     Рабочих_Дней(рабочих_Дней), Отработанных_Дней(отработанных_Дней)
     {
        Вычисление_Нач_Сумм();
        Удержание();
     }
     void Вычисление_Нач_Сумм() {
         Начисленная_сумма = (Оклад/Рабочих_Дней)*Отработанных_Дней+(Оклад*(Надбавка/100.));
     }
     void Удержание() {
         Удержанные_суммы = (Начисленная_сумма)*(100./Налог)+(Начисленная_сумма*(1/100.));
     }
     int Получить_Стаж() {
        Date result = Дата + Рабочих_Дней;
        return result.GetYear()-Дата.GetYear();
     }
};
int main() {
    srand(std::time(NULL));
    Date *n = new Date(1999, 12, 10);
    std::cout<<"N: "<<n->GetDate()<<std::endl;
    Date *m = new Date("12.2.0000");
    std::cout<<"M: "<<m->GetDate()<<std::endl;
    Date goal = *n+*m;
    Date second = goal+5;
    std::cout<<"Result: "<<goal.GetDate()<<std::endl;
    std::cout<<"Result: "<<second.GetDate()<<std::endl;
    std::cout<<second.Get_Difference(goal)<<" | days  goal:" << goal.GetDays()<<"\ndays second: "<<second.GetDays()<<std::endl;
    
    fio work = {"Анхаваых", "БекБек", "Ваолрываоды"};
    Payment *worker = new Payment(work, 10000, 0, Date("12.9.2000"), 3000, 3100);
    std::cout<<"\nСтаж батрака: "<<worker->Получить_Стаж()<<std::endl;
    return 0;
    
}
