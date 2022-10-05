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
        // 콘솔앱.
        public static void Main(string[] args)
        {
            // 3rd IHost를 생성한다.
            // 4th Run()한다(이때부터 Listen 시작한다)
            //
            CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            // 1st 각종 옵션 설정을 세팅.
            Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    //2nd Startup 클래스를 지정.. -> Configure
                    webBuilder.UseStartup<Startup>();
                });
    }
}

// Program class : 거시적인 설정 작업 수행(HTTP설정, IIS 사용 여부...)\
// Startup class : 세부적인 설정 수행(미들웨어 설정, 의존성 주입 등)
//      미들웨어 -> 소프트웨어 컴포넌트들의 모음. 미들웨어를 구성할 컴포넌트들을 조합하여, 
//                  클라이언트의 요청을 처리할 경로(Pipe line)구성 하거나 사전작업을 수행하게 할 수도 있다.
//                  이러한 처리 경로를 조립 할 때는 요청 대리자 (request delegaate)를 사용한다.