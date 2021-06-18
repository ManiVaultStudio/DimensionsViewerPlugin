let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v5.json",
    width: "container",
    height: "container",
    title: {
        text: []
    },
    config: {
        axis: {
            grid: true,
            gridColor: "#dedede"
        }
    },
    padding: 10,
    autosize:
    {
        type: "fit",
        resize: true,
    },
    "data": {
        "name": "dimensions",
        "values": []
    },
    "layer": []
}

function getRangeMark(channel, strokeWidth) {
    return {
        "mark": {
            "type": "errorband",
            //"borders": {
            //    "opacity": 0.5,
            //    "strokeWidth": strokeWidth,
            //},
            "opacity": 0.01 * 0.3 * channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "min",
                "type": "quantitative",
                "title": "Point value"
            },
            "y2": {
                "field": "max",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregateLineMark(channel, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "strokeJoin": "round",
            "opacity": 0.01 * channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        encoding: {
            x: channel.encoding.x,
            y: {
                field: "agg",
                type: "quantitative",
                title: "Point value"
            },
            color: {
                value: channel.color
            }
        }
    }
}

function getAggregatePointsMark(channel) {
    return {
        mark: {
            type: "point",
            fill: channel.color,
            opacity: 0.01 * channel.opacity,
            size: 10 * channel.primaryLineThickness
        },
        transform: [
            {
                filter: {
                    "field": "chn",
                    equal: channel.index
                }
            }
        ],
        encoding: {
            x: channel.encoding.x,
            y: {
                "field": "agg",
                type: "quantitative",
                title: "Point value"
            },
            color: {
                value: channel.color
            }
        }
    }
}

function getStdDevLineMark(channel, field, strokeWidth, strokeDash) {
    return {
        mark: {
            type: "line",
            strokeWidth: strokeWidth,
            strokeDash: strokeDash,
            strokeJoin: "round",
            opacity: 0.01 * channel.opacity,
        },
        transform: [
            {
                filter: {
                    field: "chn",
                    equal: channel.index
                }
            }
        ],
        encoding: {
            x: channel.encoding.x,
            y: {
                "field": field,
                type: "quantitative",
                title: "Point value"
            },
            color: {
                value: channel.color
            }
        }
    }
}

function getDashPattern(lineType) {
    if (lineType == "Solid")
        return [];

    if (lineType == "Dash")
        return [5, 5];

    if (lineType == "Dot")
        return [2, 2];

    if (lineType == "DashDot")
        return [5, 2, 2, 2];

    if (lineType == "DashDotDot")
        return [5, 2, 2, 2, 2, 2];

    return [];
}

function addChannel(design, channel) {
    design.data.values = design.data.values.concat(channel.dimensions);

    if (channel.profileType >= 0) {
        design.layer.push(getAggregateLineMark(channel, channel.primaryLineThickness, getDashPattern(channel.primaryLineType)));
        //design.layer.push(getAggregateBarMark(channel));

        if (channel.showPoints)
            design.layer.push(getAggregatePointsMark(channel));       
    }
    
    if (channel.bandType > 0) {
        design.layer.push(getStdDevLineMark(channel, "v1", channel.secondaryLineThickness, getDashPattern(channel.secondaryLineType)));
        design.layer.push(getStdDevLineMark(channel, "v2", channel.secondaryLineThickness, getDashPattern(channel.secondaryLineType)));
    }
    
    if (channel.showRange)
        design.layer.push(getRangeMark(channel, 1));
    
    //design.layer.push(getHistogramMark(channel));
}

function getDesign(spec) {

    design.data.values = [];
    design.layer = [];
    
    let titles = [];

    for (channelName in spec.channels) {
        let channel = spec.channels[channelName];

        titles.push(channel.datasetName);

        channel.encoding = {
            x: {
                field: spec.showDimensionNames ? 'dimName' : 'dimId',
                type: spec.showDimensionNames ? 'ordinal' : 'nominal',
                title: spec.showDimensionNames ? 'Dimension name' : 'Dimensions index',
                axis: {
                    labelAngle: spec.showDimensionNames ? -45 : 0
                }
            }
        }

        addChannel(design, channel);
    }
    /*
    design.layer.push({
        "encoding": {
            //"color": { "field": "symbol", "type": "nominal" },
            "y": {
                "field": "agg",
                "type": "quantitative"
            }
        },
        "transform": [{
            "filter": {
                "param": "hover",
                "empty": false
            }
        }],
        "mark": "point"
    });
    */
    
    design.layer.push({
        "transform": [{
            "pivot": "chn",
            "value": "min",
            "groupby": ["dimName"]
        }],
        "mark": "bar",
        "encoding": {
            "x": { "field": "dimName", "type": "nominal" },
            "opacity": {
                "value": 0
            }
        },
    });
    

    design.title.text = `[${titles.join(', ')}]`;

    return design;
}