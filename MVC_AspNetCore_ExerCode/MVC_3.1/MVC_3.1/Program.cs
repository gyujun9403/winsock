using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MVC_3._1
{
    public class Program
    {
        // �ܼ־�.
        public static void Main(string[] args)
        {
            // 3rd IHost�� �����Ѵ�.
            // 4th Run()�Ѵ�(�̶����� Listen �����Ѵ�)
            //
            CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            // 1st ���� �ɼ� ������ ����.
            Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    //2nd Startup Ŭ������ ����.. -> Configure
                    webBuilder.UseStartup<Startup>();
                });
    }
}

// Program class : �Ž����� ���� �۾� ����(HTTP����, IIS ��� ����...)\
// Startup class : �������� ���� ����(�̵���� ����, ������ ���� ��)
//      �̵���� -> ����Ʈ���� ������Ʈ���� ����. �̵��� ������ ������Ʈ���� �����Ͽ�, 
//                  Ŭ���̾�Ʈ�� ��û�� ó���� ���(Pipe line)���� �ϰų� �����۾��� �����ϰ� �� ���� �ִ�.
//                  �̷��� ó�� ��θ� ���� �� ���� ��û �븮�� (request delegaate)�� ����Ѵ�.