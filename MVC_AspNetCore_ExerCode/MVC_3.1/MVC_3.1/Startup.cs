using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.HttpsPolicy;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MVC_3._1
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // IHost를 생성. 서비스를 추가(의존성 추가?)할 때 해당 함수에 추가.
        // DI 서비스?
        //  SRP(Single Responsibility principle) : 단일 책임 원칙. 특정 기능에 책임이 발생했을 때, 문제를 찾을 곳은 단 하나의 클래스(서비스)
        //  1클래스 1책임
        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllersWithViews();
        }

        // 🌟 미들웨어 부분.🌟
        // 미들웨어 : HTTP request, response를 처리하는 중간 로직.
        // request -> pipeline(미들웨어) -> EndPoint
        // EndPoint -> pipeline(미들웨어) -> response
        // 요청이던 응답이던 미들웨어를 거친다.
        // 미들웨어에서 예외처리등을 통해 미들웨어에서 처리 가능. -> 
        // Controller에서 처리하지 않는 이유 = 컨트롤러에 있는 모든 함수마다 공통적으로 처리/확인해야할 로직들이 들어간다.
        // HTTP Request Pipeline : 특정 HTTP요청에 대하, 앱이 어떻게 동작할지 정하는 공.
        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment()) // 개발상태(디버깅모드)인지
            {
                // EndPoint등에서 에러가 발생한 경우 에러가 미들웨어로 다시 전파돔
                // = 에러가 나면 이 함수를 다시 실행하게 됨...?
                // 디버깅 모드인 경우 보여줄 페이지
                app.UseDeveloperExceptionPage();
            }
            else
            {
                // Error.cshtml로 연결.
                // UseEndpoints등에서 문제 발생 -> 미들웨어(이 함수)로 분기
                //  -> UseExceptionHandler("/Home/Error")에 의해 /Home/Error로 라우팅되는걸로 처리됨.
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            app.UseHttpsRedirection();
            // 특정 경로의 CSS, JS, 이미지등 요청된 정적 파일 요청을 응답하는 함수.
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();

            // 라우팅 대로 URL을 해석하고, 이를 컨트롤러에 넘겨주는 역할
            // MVC의 부품?
            app.UseEndpoints(endpoints =>
            {
                // MVC를 호출할 것이다.
                endpoints.MapControllerRoute(
                    // 라우팅 패턴
                    name: "default",
                    pattern: "{controller=Home}/{action=Index}/{id?}");
            });
        }
    }
}
