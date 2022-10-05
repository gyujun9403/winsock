namespace AspNetCoreMVC
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllersWithViews();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            // 어떤 HTTP요청이 왔을때, 앱이 어떻게 응답할지에 대한 일련의 과정
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            app.UseHttpsRedirection();
            app.UseStaticFiles(); // CSS, JS, 이미지 등 요청을 처리하는 함수.

            app.UseRouting();

            app.UseAuthorization();

            // UseEndpoint?? -> 라우팅 미들웨어. 라우팅 패턴 설정한다.
            // 라우팅 : 들어온 클라의 요청을 특정 MVC컨트롤러의 작업에 매칭하는 것.
            // https://localhost:7056/Home/Privacy
            // {controller=Home}     /{action=Index}/{id?}
            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}");

            app.Run();
        }
    }
}