#include "save_csv.h"

save_csv::save_csv()
{

}

QStringList save_csv::read_csv()
{
    QFile inFile("./data/data_input.csv");
    QStringList lines;/*行数据*/
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int j = 0; j < lines.size(); j++)
        {
            QString line = lines.at(j);
            QStringList split = line.split(",");/*列数据*/
            for (int col = 0; col < split.size(); col++)
            {
                cout << split.at(col).toStdString() << " ";
            }
            cout << endl;
        }
        inFile.close();
    }
    return lines;
}

void save_csv::test_csv()
{
    QFile outFile("./data/data_input.csv");
    QStringList lines;
    lines << "序号,院系,专业班级,姓名,性别,学号,考试科目,考次,考场号,座位号\n"
          << "1,信科院,智能科学与技术20-1,周为,男,202044515123,英语,6月20日下午一大,九教北301,23\n"
          << "2,信科院,计算机科学与技术20-2,欧舟,男,202044515101,英语,6月20日下午一大,九教北301,24\n"
          << "3,信科院,物联网工程20-3,付祥广,男,202044515122,英语,6月20日下午一大,九教北301,25\n"
          << "4,信科院,电子信息科学20-1,李建华,女,202044515124,英语,6月20日下午一大,九教北301,26";

    /*如果以ReadWrite方式打开，不会把所有数据清除，如果文件原来的长度为100字节，写操作写入了30字节，那么还剩余70字节，并且这70字节和文件打开之前保持一直*/
    if (outFile.open(QIODevice::WriteOnly))
    {
        for (int i = 0; i < lines.size(); i++)
        {
            outFile.write(lines[i].toStdString().c_str());/*写入每一行数据到文件*/
        }
        outFile.close();
    }
}

//void save_csv::write_csv(save_csv::stu_info s)
//{

//}

