using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using WASAuth.Models;

// 나이와 VIP여부에 따른 입장 Requirment
public class CanEnterRequirment : IAuthorizationRequirement
{
    // 조건 검사해야할 조건은 이 핸들을 포함하는 Requirement에 있는게 맞다.
    public int MinAge { get; }
    public int MaxAge { get; }

    public CanEnterRequirment(int minAge, int maxAge)
    {
        MinAge = minAge;
        MaxAge = maxAge;
    }
}

// CanEnterRequirment의 핸들
public class AgeHandler : AuthorizationHandler<CanEnterRequirment>
{
    // 추상클래스의 구현해줘야 하는  함수(자동완성)
    protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, CanEnterRequirment requirement)
    {
        // 정책 검사시, 이 함수로 분기된다.
        // context : 클라의 정보. -> 클라의 데이터를 확인하여 조건이 맞는지 확인.
        // requirement : 이 핸들을 포함하는 Requirement.

        // 
        Claim claim = context.User.Claims.FirstOrDefault(c => c.Type == "Age");
        if (claim != null)
        {
            int age = int.Parse(claim.Value);
            if (age >= requirement.MinAge && age <= requirement.MaxAge)
            {
                //
                context.Succeed(requirement); // 해당 requirement는 통과했음
            }
            else
            {
                // Requirement가 만족되지 않았으므로 아무것도 하지 않겠다.
                return Task.CompletedTask;
            }
        }
        throw new NotImplementedException();
    }
}

public class IsVipHandler : AuthorizationHandler<CanEnterRequirment>
{
    protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, CanEnterRequirment requirement)
    {
        Claim claim = context.User.Claims.FirstOrDefault(c => c.Type == "IsVip");
        if (claim != null)
        {
            bool isVip = bool.Parse(claim.Value);
            if (isVip == true)
            {
                context.Succeed(requirement);
            }
            else
            {
                return Task.CompletedTask;
            }
        }
        throw new NotImplementedException();
    }
}

//------------------------------------------------------------------

// 블랙리스트 여부에 따른 입장 Requirement
public class IsNotBlackListRequirment : IAuthorizationRequirement
{ }

public class IsNotBlackListHandler : AuthorizationHandler<IsNotBlackListRequirment>
{
    protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, IsNotBlackListRequirment requirement)
    {
        Claim claim = context.User.Claims.FirstOrDefault(c => c.Type == "IsBlackList");
        if (claim != null)
        {
            bool isBaned = bool.Parse(claim.Value);
            if (isBaned == true)
            {
                // 다른 Rerquirement와 상관없이 그냥 실패. Requirement인자로 안받음.
                context.Fail();
            }
            else
            {
                return Task.CompletedTask;
            }
        }
        throw new NotImplementedException();
    }
}

namespace WASAuth.Controllers
{
    [Authorize]
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Index()
        {
            return View();
        }

        [Authorize("AdminPolicy")]
        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
