#include"B_time.h"
void IntToChar(int x, char* Str)//ת��int���ַ���
{
    int t;
    char* Ptr, Buf[5];
    int i = 0;
    Ptr = Str;
    if (x < 10)  // ������С��10��ת��Ϊ0x��ʽ,�����0
    {
        (*Ptr++) = '0';
        (*Ptr++) = x + 0x30;//0x30 == '0';
    }
    else
    {
        while (x > 0)
        {
            t = x % 10;
            x = x / 10;
            Buf[i++] = t + 0x30; // ��0x30 == '0';��
        }
        i--;
        for (; i >= 0; i--)   // ���õ����ַ�������
        {
            *(Ptr++) = Buf[i];
        }
    }
    *Ptr = '\0';
}





Date_C B_Time_C(void) {//�����ַ�����ʱ��ĺ���
    time_t  timep;
    Date_C date;
    struct tm* p;
    time(&timep);//��ȡ��1970������˶����룬����time_t���͵�timep
    p = localtime(&timep);//ȡ�ô�1900�꿪ʼ��ʱ��
    IntToChar(1900 + p->tm_year, date.B_Year);//year
    IntToChar(1 + p->tm_mon, date.B_Months);//months
    IntToChar(1 + p->tm_mday, date.B_Day);//day
    IntToChar(1 + p->tm_hour, date.B_Hour);//hour
    IntToChar(1 + p->tm_min, date.B_Minute);//minute
    IntToChar(1 + p->tm_sec, date.B_Second);//second

    return date;
}

Date B_Time_I(void) {//��������ʱ��ĺ���
    time_t  timep;
    Date date;
    struct tm* p;
    time(&timep);//��ȡ��1970������˶����룬����time_t���͵�timep
    p = localtime(&timep);//ȡ�ô�1900�꿪ʼ��ʱ��
    date.B_Year = 1900 + p->tm_year;//year
    date.B_Months = 1 + p->tm_mon;//months
    date.B_Day = 1 + p->tm_mday;//day
    date.B_Hour = 1 + p->tm_hour;//hour
    date.B_Minute = 1 + p->tm_min;//minute
    date.B_Second = 1 + p->tm_sec;//second

    return date;
}

int B_DateIsSmall(Date x, Date y) {//�Ƚ�����ʱ�䣬x<y?1:0 ;
    if (x.B_Year < y.B_Year) {
        return 1;
    }
    else if (x.B_Year > y.B_Year) {
        return 0;
    }
    else if (x.B_Year == y.B_Year) {
        if (x.B_Months < y.B_Months) {
            return 1;
        }
        else if (x.B_Months > y.B_Months) {
            return 0;
        }
        else if (x.B_Months == y.B_Months) {
            if (x.B_Day < y.B_Day) {
                return 1;
            }
            else if (x.B_Day > y.B_Day) {
                return 0;
            }
            else if (x.B_Day == y.B_Day) {
                return 1;
            }
        }
    }

}
int B_DayPassed(Date pre, Date lat) {//�����������ڼ���������������ݴ��󣬷���-1��
    int y2, m2, d2;
    int y1, m1, d1;
    if (B_DateIsSmall(lat, pre)) {
        return -1;
    }
    else {
        m1 = (pre.B_Months + 9) % 12;
        y1 = pre.B_Year - m1 / 10;
        d1 = 365 * y1 + y1 / 4 - y1 / 100 + y1 / 400 + (m1 * 306 + 5) / 10 + (pre.B_Day - 1);

        m2 = (lat.B_Months + 9) % 12;
        y2 = lat.B_Year - m2 / 10;
        d2 = 365 * y2 + y2 / 4 - y2 / 100 + y2 / 400 + (m2 * 306 + 5) / 10 + (lat.B_Day - 1);

        return (d2 - d1);
    }
}