// code sample of hacking old script
;({
  "type": "Program",
  "body": [
    {
      "type": "ExpressionStatement",
      "expression": {
        "type": "AssignmentExpression",
        "operator": "=",
        "left": {
          "type": "MemberExpression",
          "computed": false,
          "object": {
            "type": "Identifier",
            "name": "module"
          },
          "property": {
            "type": "Identifier",
            "name": "exports"
          }
        },
        "right": {
          "type": "ObjectExpression",
          "properties": [
            {
              "type": "Property",
              "key": {
                "type": "Identifier",
                "name": "data"
              },
              "computed": false,
              "value": {
                // "type": "FunctionExpression",
                // "id": null,
                // "params": [],
                // "defaults": [],
                // "body": {
                  // "type": "BlockStatement",
                  // "body": [
                    // {
                      // "type": "ReturnStatement",
                      // "argument": {
                        "type": "ObjectExpression",
                        "properties": []
                      // }
                    // }
                  // ]
                // },
                // "generator": false,
                // "expression": false
              },
              "kind": "init",
              "method": false,
              "shorthand": false
            }
          ]
        }
      }
    }
  ],
  "sourceType": "script"
})

var LEFT_MODULE_EXPORTS_AST = {
  "type": "MemberExpression",
  "computed": false,
  "object": {
    "type": "Identifier",
    "name": "module"
  },
  "property": {
    "type": "Identifier",
    "name": "exports"
  }
}

function removeAllLoc(ast) {
  ast = JSON.parse(JSON.stringify(ast))
  function remove(o) {
    if (Array.isArray(o)) {
      o.forEach(remove)
    }
    else if (typeof o === 'object') {
      for (var i in o) {
        if (i === 'loc') {
          delete o[i]
        }
        if (i === 'range') {
          delete o[i]
        }
        else {
          if (typeof o[i] === 'object') {
            remove(o[i])
          }
        }
      }
    }
  }
  remove(ast)
  return ast
}

function findDataValue(ast) {
  var exp, left, right, dataValue
  if (ast && ast.body && ast.body.length) {
    ast.body.forEach(function (bodyItem) {
      if (bodyItem.type === 'ExpressionStatement') {
        exp = bodyItem.expression
        if (exp.type === 'AssignmentExpression' && exp.operator === '=') {
          left = removeAllLoc(exp.left || {})
          if (JSON.stringify(left) === JSON.stringify(LEFT_MODULE_EXPORTS_AST)) {
            right = exp.right
            if (right.type === 'ObjectExpression') {
              right.properties.some(function (prop) {
                if (prop.type === 'Property' && prop.key && prop.key.name === 'data') {
                  if (prop.value && prop.value.type === 'ObjectExpression') {
                    dataValue = prop
                    return true
                  }
                }
              })
            }
          }
        }
      }
    })
  }
  return dataValue
}

function convertValueAst(value) {
  var data = {
    type: "FunctionExpression",
    id: null,
    params: [],
    defaults: [],
    body: {
      type: "BlockStatement",
      body: [
        {
          type: "ReturnStatement",
          argument: value
        }
      ]
    },
    generator: false,
    expression: false
  }
  return data
}
function format(code, needCodegen) {
  return code
}

function formatBetter(code) {
  return code
}

exports.fix = formatBetter
exports.formatWhenFix = format
